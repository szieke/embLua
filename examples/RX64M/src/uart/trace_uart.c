
#include "trace_uart.h"
#include "iodefine.h"


typedef struct
{
  uint8_t* receive;/*!< Receive buffer (ring buffer).*/
  uint16_t receiveSize;/*!< The size of recive.*/
  uint8_t* transmit; /*!< The transmit buffer (ring buffer).*/
  uint16_t transmitSize;/*!< The size of transmit.*/
  uint16_t bytesInReceiveBuffer; /*!< The number of byte in the receive buffer.*/
  uint16_t receiveHead; /*!< The head of the receive buffer.*/
  uint16_t receiveTail; /*!< The tail of the receive buffer.*/
  uint16_t transmitHead; /*!< The head of the transmit buffer.*/
  uint16_t transmitTail; /*!< The tail of the transmit buffer.*/
  uint16_t bytesInTransmitBuffer; /*!< The number of byte in the transmit buffer.*/

  bool_t sendingActive;

} trace_uartData_t;

static trace_uartData_t g_uartData;

#define UART_INT_PRIO     (0x01u)     // Lowest priority
#define BAUDRATE (115200u)
#define FSYS 120000000u        /* Assume 120MHz*/
#define PCLK_UART  (FSYS/2u)
#define BAUDDIVIDE (((PCLK_UART + (BAUDRATE * 16u)) / (BAUDRATE * 32u)) - 1u)

#define UART_RX_VECT     98u
#define UART_TX_VECT     99u

void trace_txInterrupt(void)
{
  if (g_uartData.sendingActive)
  {
    g_uartData.bytesInTransmitBuffer--;

    if (g_uartData.bytesInTransmitBuffer != 0)
    {
      SCI7.TDR = g_uartData.transmit[g_uartData.transmitTail];

      g_uartData.transmitTail++;
      if (g_uartData.transmitTail >= g_uartData.transmitSize)
      {
        g_uartData.transmitTail = 0U;
      }
    }
    else
    {
      g_uartData.sendingActive = FALSE;
    }
  }
}

#pragma interrupt (_ISR_Rx(vect=UART_RX_VECT)) // Rx data interrupt
static void _ISR_Rx(void)
{

  if (g_uartData.bytesInReceiveBuffer != g_uartData.receiveSize)
  {/*Receive buffer is not full.*/

    g_uartData.receive[g_uartData.receiveHead] = SCI7.RDR;
    g_uartData.bytesInReceiveBuffer++;

    g_uartData.receiveHead++;
    if (g_uartData.receiveHead >= g_uartData.receiveSize)
    {
      g_uartData.receiveHead = 0U;
    }

    if (g_uartData.bytesInReceiveBuffer == g_uartData.receiveSize)
    {/*Receive buffer is full.*/
    }
  }

}

#pragma interrupt (_ISR_Tx(vect=UART_TX_VECT))  // TX data empty interrupt
static void _ISR_Tx(void)
{
  if (g_uartData.sendingActive)
  {
    g_uartData.bytesInTransmitBuffer--;

    if (g_uartData.bytesInTransmitBuffer != 0)
    {
      SCI7.TDR = g_uartData.transmit[g_uartData.transmitTail];

      g_uartData.transmitTail++;
      if (g_uartData.transmitTail >= g_uartData.transmitSize)
      {
        g_uartData.transmitTail = 0U;
      }
    }
    else
    {
      g_uartData.sendingActive = FALSE;
    }
  }
}

static uint32_t trace_writePossible(void)
{
  return g_uartData.transmitSize - g_uartData.bytesInTransmitBuffer;
}

static uint32_t trace_writeInternal(const uint8_t buffer[], const uint32_t bufferSize)
{
  uint32_t writtenBytes = 0U;
  uint32_t bufferIndex = 0U;

  uint32_t freeSpace = trace_writePossible();

  if (freeSpace >= bufferSize)
  {
    writtenBytes = bufferSize;
  }
  else
  {
    writtenBytes = freeSpace;
  }

  if (writtenBytes != 0U)
  {

    IEN( SCI7, TXI7 )= 0;

    if (!g_uartData.sendingActive)
    {
      SCI7.TDR = buffer[0];
      bufferIndex = 1;
      g_uartData.sendingActive = TRUE;

    }

    for (; bufferIndex < writtenBytes; bufferIndex++)
    {
      g_uartData.transmit[g_uartData.transmitHead] = buffer[bufferIndex];

      g_uartData.transmitHead++;
      if (g_uartData.transmitHead >= g_uartData.transmitSize)
      {
        g_uartData.transmitHead = 0U;
      }

    }

    g_uartData.bytesInTransmitBuffer += writtenBytes;

    IEN( SCI7, TXI7 ) = 1;

  }/*if (writtenBytes != 0U)*/

  return writtenBytes;
}


uint32_t trace_write(const uint8_t buffer[], const uint32_t bufferSize, bool_t waitUntilAllBytesAreInBuffer)
{

  uint32_t bytesSend = 0;

  if (waitUntilAllBytesAreInBuffer)
  {
    do
    {
      bytesSend += trace_writeInternal(&buffer[bytesSend], bufferSize - bytesSend);
      if ((bytesSend < bufferSize))
      {
      }
    }
    while (bytesSend < bufferSize);
  }
  else
  {
    bytesSend = trace_writeInternal(buffer, bufferSize);
  }

  return bytesSend;
}

void trace_initializeUart()
{

  g_uartData.receiveSize = TRACE_RECEIVE_BUFFER_SIZE;
  g_uartData.transmitSize = TRACE_TRANSMIT_BUFFER_SIZE;
  g_uartData.receiveTail = 0U;
  g_uartData.receiveHead = 0U;
  g_uartData.transmitTail = 0U;
  g_uartData.transmitHead = 0U;
  g_uartData.bytesInReceiveBuffer = 0;
  g_uartData.bytesInTransmitBuffer = 0;
  g_uartData.sendingActive = FALSE;

  SYSTEM.PRCR.WORD = 0xA50Bu; // Protect off

  volatile unsigned int i;

  // Enable UART Module
  SYSTEM.MSTPCRB.BIT.MSTPB24 = 0;

  MPC.PWPR.BYTE = 0x00u; // Disable write protect
  MPC.PWPR.BYTE = 0x40u; // Enable write access

  MPC.P90PFS.BYTE = 0x0Au; // Set pin function register to uart function
  MPC.P92PFS.BYTE = 0x0Au; // Set pin function register to uart function
  MPC.PWPR.BYTE = 0x80u; // Enable write protect
  PORT9.PMR.BYTE |= 0x05u; // Set pin to peripheral function

  // Reset, disable Reception and transmission
  SCI7.SCR.BYTE = 0x00u;

  // Set transmission mode
  SCI7.SMR.BYTE = 0x00u;

  // Set transfer direction, LSB first, 8 bit data length
  SCI7.SCMR.BYTE = 0x10u;

  // Set baudrate
  SCI7.BRR = (uint32_t) (BAUDDIVIDE);
  //
  // Wait for SCI Settling time (1 bit cycle)
  //
  for (i = 0u; i < 3000u; i++)
  {
  }

  // Clear status
  SCI7.SSR.BYTE = 0x00u;

  /* Set interrupt priority */
  IPR( SCI7, RXI7 )= UART_INT_PRIO;
  IPR( SCI7, TXI7 )= UART_INT_PRIO;

  /* Clear interrupt flag */
  IR( SCI7, RXI7 )= 0;
  IR( SCI7, TXI7 )= 0;

  /* Enable SCI7 interrupt */
  IEN( SCI7, RXI7 )= 1;
  IEN( SCI7, TXI7 )= 1;

  //
  // Finally enable Rx and Tx interrupts and enable uart Rx and Tx
  //
  SCI7.SCR.BIT.TIE = 1; // Tx interrupt enable
  SCI7.SCR.BIT.RIE = 1; // Rx interrupt enable
  SCI7.SCR.BIT.TE = 1; // Tx enable
  SCI7.SCR.BIT.RE = 1; // Rx enable

  SYSTEM.PRCR.WORD = 0xA500u; // Protect on

}

uint32_t trace_peak(void)
{
  return g_uartData.bytesInReceiveBuffer;
}

uint32_t trace_read(uint8_t buffer[], const uint32_t bufferSize)
{
  uint32_t read_bytes = 0U;

  if (g_uartData.bytesInReceiveBuffer >= bufferSize)
  {
    read_bytes = bufferSize;
  }
  else
  {
    read_bytes = g_uartData.bytesInReceiveBuffer;
  }

  IEN( SCI7, RXI7 )= 0;

  if (0U != read_bytes)
  {
    for (uint32_t i = 0U; i < read_bytes; i++)
    {
      buffer[i] = g_uartData.receive[g_uartData.receiveTail];

      g_uartData.receiveTail++;
      if (g_uartData.receiveTail >= g_uartData.receiveSize)
      {
        g_uartData.receiveTail = 0U;
      }
    }

    g_uartData.bytesInReceiveBuffer -= read_bytes;

  }

  IEN( SCI7, RXI7 )= 1;

  return read_bytes;
}

