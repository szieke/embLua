/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*       (c) 1995 - 2016 SEGGER Microcontroller GmbH & Co. KG         *
*                                                                    *
*       Internet: segger.com  Support: support_embos@segger.com      *
*                                                                    *
**********************************************************************
*                                                                    *
*       embOS * Real time operating system for microcontrollers      *
*                                                                    *
*       Please note:                                                 *
*                                                                    *
*       Knowledge of this file may under no circumstances            *
*       be used to write a similar product or a real-time            *
*       operating system for in-house use.                           *
*                                                                    *
*       Thank you for your fairness !                                *
*                                                                    *
**********************************************************************
*                                                                    *
*       OS version: 4.24                                             *
*                                                                    *
**********************************************************************

----------------------------------------------------------------------
File    : JLINKRX_Process.c
Purpose : Data handler for RX J-Link communication over memory

Layout of communication area:

  +----------+  TOS - TX_SIZE - RX_SIZE - 6
  |          |
  | RX_BUF   |
  |          |
  +----------+  TOS - TX_SIZE - 6
  | RX_CNT   |
  +----------+  TOS - TX_SIZE - 5
  | HOST_ACT |
  +----------+  TOS - TX_SIZE - 4
  | TX_CNT   |
  +----------+  TOS - TX_SIZE - 3
  |          |
  | TX_BUF   |
  |          |
  +----------+  TOS - 3
  | TX_SIZE  |
  +----------+  TOS - 2
  | RX_SIZE  |
  +----------+  TOS - 1
  | PROT_ID  |
  +----------+  TOS

TOS       Initial top of stack as defined by linker settings (top of CSTACK)
PROT_ID   Magic number indicating the start of communication area
RX_SIZE   Size of receiving buffer in bytes
TX_SIZE   Size of sending buffer in bytes
TX_BUF    Sending buffer
TX_CNT    Number of bytes in sending buffer
HOST_ACT  Set to one by host to indicate it is still active
RX_CNT    Number of bytes in the receiving buffer
RX_BUF    Receiving buffer

Notes:
  embOSView communication for RX via J-Link assumes that the used RX CPU
  has RAM memory at address 0x00 and the RAM memory 0x00 - 0x03 is not
  used by the application. This can be achieved by starting the RAM in
  the linker file at address 0x04. This is already done for all embOS
  linker files. The same is true for IAR linker files (but for another
  reason: Using RAM memory at 0x00 is a bad idea because a pointer value
  of 0x00 could be a valid value when it is pointing to the start of RAM.
---------------------------END-OF-HEADER------------------------------
*/

#include "RTOS.h"
#include "JLINKRX.h"

/*********************************************************************
*
*       Defines, configurable
*
**********************************************************************
*/

/*********************************************************************
*
*       Sizes of receiving and sending buffer.
*/
#define BUF_SIZE      32u
#define RX_BUF_SIZE   8u
#define TX_BUF_SIZE   18u

/*********************************************************************
*
*       Defines, non- configurable
*
**********************************************************************
*/
#define MAGIC_NUM     0x53u /* Identifies the start of our communication area. */

#define PROT_ID       (*(volatile OS_U8 *)(_BaseAddr - 1u))                 /* Id of the protocol. Always set to MAGIC_NUM */
#define RX_SIZE       (*(volatile OS_U8 *)(_BaseAddr - 2u))                 /* Size of receiving buffer in bytes */
#define TX_SIZE       (*(volatile OS_U8 *)(_BaseAddr - 3u))                 /* Size of sending buffer in bytes */
#define TX_CNT        (*(volatile OS_U8 *)((_BaseAddr - TX_BUF_SIZE) - 4u)) /* Stores the number of bytes we send to host */
/* Set by host to a non-null value when it connects to target.
 * The target sets this to null when it detects a communication timeout.
 */

#define HOST_CON      (*(volatile OS_U8 *)((_BaseAddr - TX_BUF_SIZE) - 5u))
#define RX_CNT        (*(volatile OS_U8 *)((_BaseAddr - TX_BUF_SIZE) - 6u))  /* Stores the number of bytes the host sent to us */

#define TX_TIMEOUT    1000u  /* Time to wait for host to fetch the data from sending buffer (in ms) */

#define RX_RAM_START_ADDR  0x00u

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

/* Pointer to user callbacks */
void (* _pfOnRx) (OS_U8 Data);
void (* _pfOnTx) (void);
OS_INT (* _pfGetNextChar) (void);

static OS_U8   _Buffer[BUF_SIZE];
static OS_U32  _BaseAddr;
static OS_U32  _TxIsPending;    /* Set when there is a character waiting to be sent */
static OS_U8   _TxPendingData;  /* Holds tha character waiting to be sent */
static OS_U32  _IsInited;       /* Set when the communication is initialised */
static OS_U8*  _pRxBuf;         /* Start of receiving buffer */
static OS_U8*  _pTxBuf;         /* Start of sending buffer */

/* Supervises the connection to host */
static OS_U32  _TxTimeout;      /* Set when the host fails to fetch our data */
static OS_U32  _TxTimeoutTimer;
static OS_U32  _TxBufLocked;    /* Serialises the access to our sending buffer */

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

/*********************************************************************
*
*       _RetriggerTimer
*
* Function description
*/
static void _RetriggerTimer(void) {
  _TxTimeoutTimer = TX_TIMEOUT;
}

/*********************************************************************
*
*       _HandleTxTimeout
*
* Function description
*   Sets the connection to host to inactive when the host fails to
*   fetch the data we send to it. Called when the send timeout
*   expires.
*/
static void _StopTimer(void) {
  _TxTimeoutTimer = 0u;
}

/*********************************************************************
*
*       _Init
*
* Function description
*   Performs initalisation of memory locations used for communication.
*/
static void _Init(void) {
  _BaseAddr = ((OS_U32)_Buffer + BUF_SIZE);
  HOST_CON  = 0u;
  RX_SIZE   = RX_BUF_SIZE;
  TX_SIZE   = TX_BUF_SIZE;
  RX_CNT    = 0u;
  TX_CNT    = 0u;
  PROT_ID   = MAGIC_NUM;
  _pTxBuf   = (OS_U8 *)((_BaseAddr - TX_BUF_SIZE) - 3u);
  _pRxBuf   = (OS_U8 *)(((_BaseAddr - TX_BUF_SIZE) - RX_BUF_SIZE) - 6u);
  _TxIsPending  = 0u;
  //
  // Write the base address in RX RAM memory at fixed address 0x00
  //
  *((OS_U32*)RX_RAM_START_ADDR) = (OS_U32)_BaseAddr;
}

/*********************************************************************
*
*       _LockTxBuf
*
* Function description
*   Gains exclusive access to sending buffer.
*
* Return value
*   1   Sending buffer locked
*   0   Sending buffer couldn't be locked as already in use.
*/
static OS_U32 _LockTxBuf(void) {
  OS_U32 Locked;

  Locked = 0u;
  OS_DI();
  if (!_TxBufLocked) {
    _TxBufLocked = 1u;
    Locked = 1u;
  }
  OS_RestoreI();
  return Locked;
}

/*********************************************************************
*
*       _UnlockTxBuf
*
* Function description
*   Releases the exclusive access to sending buffer.
*/
static void _UnlockTxBuf(void) {
  _TxBufLocked = 0u;
}

/*********************************************************************
*
*       _Receive
*
*  Function description
*    Performs Command / data read from host
*/
static void _Receive(void) {
  OS_U32 i;
  OS_U8  *pBuf;

  if (RX_CNT > 0u) {    /* Data received ? */
    if (_pfOnRx != NULL) {
      pBuf = _pRxBuf + (RX_BUF_SIZE - RX_CNT);
      for (i = 0u; i < RX_CNT; ++i) {
        _pfOnRx(*pBuf);
        ++pBuf;
      }
    }
    RX_CNT = 0u;
  }
}

/*********************************************************************
*
*       _FillTxBuf
*
*  Function description
*    Stores bytes in the sending buffer.
*/
static void _FillTxBuf(OS_U8 Data) {
  OS_U32 Cnt;
  OS_INT Byte;
  OS_U8  *pBuf;

  Cnt   = 0u;
  pBuf  = _pTxBuf;
  *pBuf = Data;
  ++pBuf;
  ++Cnt;
  if (_pfGetNextChar != NULL) {
    /* Get more bytes from the communication state machine
     * until the sending buffer is full.
     */
    while (1) {
      if (Cnt >= TX_BUF_SIZE) {
        break;
      }
      Byte = _pfGetNextChar();
      if (Byte < 0) {
        break;
      }
      *pBuf = (OS_U8)Byte;
      ++pBuf;
      ++Cnt;
    }
  }
  OS_DI();
  _RetriggerTimer();
  TX_CNT = (OS_U8)Cnt;
  OS_RestoreI();
}

/*********************************************************************
*
*       _DropTxData
*
*  Function description
*    Empties the sending buffer of embOS.
*/
static void _DropTxData(void) {
  if (_pfGetNextChar != NULL) {
    while (_pfGetNextChar() >= 0) {
    }
  }
}

/*********************************************************************
*
*       _Send
*
*  Function description
*    Sends data back to host if embOSView is ready to receive data.
*/
static void _Send(void) {
  if (TX_CNT == 0u) {        /* Can we send data? */
    _StopTimer();
    if (_TxIsPending) {
      _FillTxBuf(_TxPendingData);
      _TxIsPending = 0u;
    } else {
      if (_pfOnTx != NULL) {
        if (_LockTxBuf()) {
          _pfOnTx();
          _UnlockTxBuf();
        }
      }
    }
  }
}

/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/

/*********************************************************************
*
*       JLINKRX_Process
*
*  Function description
*    This function should be called more or less regularily to allow
*    memory reads while the application progam is running.
*    The more often it is called, the higher the transfer speed.
*/
void JLINKRX_Process(void) {
  if (OS_IsRunning()) {         /* No communication until the embOS starts */
    if (!_IsInited) {
      _Init();
      _IsInited = 1u;
    }
    if (HOST_CON) {             /* Do nothing until the host connects to us */
      //
      // Handle Timeout timer
      //
      if (_TxTimeoutTimer != 0u) {
        _TxTimeoutTimer--;
        if (_TxTimeoutTimer == 0u) {
          _TxTimeout = 1;
        }
      }

      if (_TxTimeout) {
        HOST_CON = 0u;
        _TxTimeout = 0u;
        _TxIsPending = 0u;
        _DropTxData();
        RX_CNT = 0u;              /* Drop all bytes form receiving buffer. */
      } else {
        _Receive();
        _Send();
      }
    }
  }
}

/*********************************************************************
*
*       JLINKRX_SendChar
*
*  Function description
*    Send data to the host. This function is non-blocking.
*    If data can not be send it is stored in a buffer
*    and sent later, when the handler is called.
*/
void JLINKRX_SendChar(OS_U8 Data) {
  if (OS_IsRunning()) {         /* No communication until the embOS starts */
    if (!_IsInited) {
      _Init();
      _IsInited = 1u;
    }
    if (HOST_CON) {             /* Do nothing until embOSView connects to us */
      if (TX_CNT == 0u) {
        if (_LockTxBuf()) {
          _FillTxBuf(Data);
          _UnlockTxBuf();
        } else {
          _TxIsPending = 1u;
          _TxPendingData = Data;
        }
      } else {
        _TxIsPending = 1u;
        _TxPendingData = Data;
      }
    } else {
      //
      // Host not connected, drop characters
      //
      _DropTxData();
    }
  } else {
    //
    // embOS not started, drop characters
    //
    _DropTxData();
  }
}

/*********************************************************************
*
*       JLINKRX_SetpfOnRx
*/
void JLINKRX_SetpfOnRx(void (* pf)(OS_U8 Data)) {
  _pfOnRx = pf;
}

/*********************************************************************
*
*       JLINKRX_SetpfOnTx
*/
void JLINKRX_SetpfOnTx(void (* pf)(void)) {
  _pfOnTx = pf;
}

/*********************************************************************
*
*       JLINKRX_SetpfGetNextChar
*/
void JLINKRX_SetpfGetNextChar(OS_INT (* pf)(void)) {
  _pfGetNextChar = pf;
}

/****** End Of File *************************************************/
