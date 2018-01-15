/**
 * Includes the API functions and defines of the uart driver.
 */

#include "trace_uart.h"


/*!< Enthaelt alle Daten fuer einen UART.*/
typedef struct
{
  uint8_t receive[TRACE_RECEIVE_BUFFER_SIZE];/*!< Der Empfangsbuffer.*/
  uint32_t receiveSize;/*!< Die Groesse von Receive.*/
  uint8_t transmit[TRACE_TRANSMIT_BUFFER_SIZE]; /*!< Der Sendebuffer.*/
  uint32_t transmitSize;/*!< Die Groesse von Transmit.*/
  uint32_t bytesInReceiveBuffer; /*!< Die Anzahl der Bytes im Empfangsspeicher.*/
  uint32_t receiveHead; /*!< Der Kopf vom Empfangsbuffer.*/
  uint32_t receiveTail; /*!< Das Ende vom Empfangsbuffer.*/
  uint32_t transmitHead; /*!< Der Kopf vom Sendebuffer.*/
  uint32_t transmitTail; /*!< Das Ende vom Sendebuffer.*/
  uint32_t bytesInTransmitBuffer; /*!< Die Anzahl der Bytes im Sendespeicher.*/

  bool_t sendingActive;

} trace_uartData_t;

/*!< Beinhaltet alle UART Datenstrukturen.*/
static trace_uartData_t g_uartData;

#define EVENACCESS             __evenaccess
#define IER0C                (*(volatile EVENACCESS uint8_t*)  (0x0008720Cu))
#define SCI7_BASE_ADDR       (0x0008A0E0u)
#define OS_UART_BASE_ADDR  (SCI7_BASE_ADDR)
#define OS_UART_TDR          (*(volatile EVENACCESS uint8_t*) (OS_UART_BASE_ADDR + 0x03u))
#define OS_UART_IER_RX     (IER0C)
#define OS_UART_IER_TX     (IER0C)
#define OS_UART_IER_RX_BIT (2u)
#define OS_UART_IER_TX_BIT (3u)

void trace_rxInterrupt(uint8_t data)
{
  if (g_uartData.bytesInReceiveBuffer != g_uartData.receiveSize)
  {/*Der Empfangspeicher ist nicht voll.*/

    /*Lese das empfangene Byte aus dem Datenregister.*/
    g_uartData.receive[g_uartData.receiveHead] = data;
    g_uartData.bytesInReceiveBuffer++;

    /*Erhoehe den Kopf des Empfangsspeichers um 1.*/
    g_uartData.receiveHead++;
    if (g_uartData.receiveHead >= g_uartData.receiveSize)
    {
      g_uartData.receiveHead = 0U;
    }

    if (g_uartData.bytesInReceiveBuffer == g_uartData.receiveSize)
    {/*Der Empfangspeicher ist voll.*/
    }
  }

}

void trace_txInterrupt(void)
{
  if (g_uartData.sendingActive)
  {
    g_uartData.bytesInTransmitBuffer--;

    if (g_uartData.bytesInTransmitBuffer != 0)
    {
      OS_UART_TDR = g_uartData.transmit[g_uartData.transmitTail];

      /*Erhoehe das Ende des Sendespeichers um 1.*/
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


/*********************************************************************
 *
 *       Compiler dependent defines
 *
 **********************************************************************
 */
#ifdef __ICCRX__
#define EVENACCESS
#define WAIT_FOR_INTERRUPT()   __wait_for_interrupt()
#endif

#ifdef __RX
#define EVENACCESS             __evenaccess
#define WAIT_FOR_INTERRUPT()   wait()
#endif

#ifdef __GNUC__
#define EVENACCESS
#define WAIT_FOR_INTERRUPT()   __builtin_rx_wait()
#endif

/*********************************************************************
 *
 *   Sfrs used in RTOSInit
 *
 **********************************************************************
 */

/****** Protect register  ********************************************/
#define PRCR                 (*(volatile EVENACCESS uint16_t*) (0x000803FEu))

/****** Interrupt controller       **********************************/
#define IRR_BASE_ADDR        (0x00087000u)
#define ISEL_BASE_ADDR       (0x00087100u)

#define IER03                (*(volatile EVENACCESS uint8_t*)  (0x00087203u))
#define IER0C                (*(volatile EVENACCESS uint8_t*)  (0x0008720Cu))

#define IPR04                (*(volatile EVENACCESS uint8_t*)  (0x00087304u))
#define IPR98                (*(volatile EVENACCESS uint8_t*)  (0x00087362u))
#define IPR99                (*(volatile EVENACCESS uint8_t*)  (0x00087363u))

/****** SYSTEM module  **********************************************/
#define SYSTEM_BASE_ADDR     (0x00080000u)
#define SYSCR0               (*(volatile EVENACCESS uint16_t*) (SYSTEM_BASE_ADDR + 0x006u))
#define MSTPCRA              (*(volatile EVENACCESS uint32_t*) (SYSTEM_BASE_ADDR + 0x010u))
#define MSTPCRB              (*(volatile EVENACCESS uint32_t*) (SYSTEM_BASE_ADDR + 0x014u))
#define MSTPCRC              (*(volatile EVENACCESS uint32_t*) (SYSTEM_BASE_ADDR + 0x018u))
#define MSTPCRD              (*(volatile EVENACCESS uint32_t*) (SYSTEM_BASE_ADDR + 0x01Cu))

#define SCKCR                (*(volatile EVENACCESS uint32_t*) (SYSTEM_BASE_ADDR + 0x020u))
#define SCKCR2               (*(volatile EVENACCESS uint16_t*) (SYSTEM_BASE_ADDR + 0x024u))
#define SCKCR3               (*(volatile EVENACCESS uint16_t*) (SYSTEM_BASE_ADDR + 0x026u))
#define PLLCR                (*(volatile EVENACCESS uint16_t*) (SYSTEM_BASE_ADDR + 0x028u))
#define PLLCR2               (*(volatile EVENACCESS uint8_t*)  (SYSTEM_BASE_ADDR + 0x02Au))
#define MOSCCR               (*(volatile EVENACCESS uint8_t*)  (SYSTEM_BASE_ADDR + 0x032u))
#define SOSCCR               (*(volatile EVENACCESS uint8_t*)  (SYSTEM_BASE_ADDR + 0x033u))
#define MOSCWTCR             (*(volatile EVENACCESS uint8_t*)  (SYSTEM_BASE_ADDR + 0x0A2u))

/****** MPC used for SDRAM init *************************************/
#define MPC_BASE_ADDR        (0x0008C100u)
#define PFAOE0               (*(volatile EVENACCESS uint8_t*) (MPC_BASE_ADDR + 0x04u))
#define PFAOE1               (*(volatile EVENACCESS uint8_t*) (MPC_BASE_ADDR + 0x05u))
#define PFBCR0               (*(volatile EVENACCESS uint8_t*) (MPC_BASE_ADDR + 0x06u))
#define PFBCR1               (*(volatile EVENACCESS uint8_t*) (MPC_BASE_ADDR + 0x07u))

/****** BSC for SDRAM init *************************************/
#define BSC_BASE_ADDR        (0x00081300u)
#define SDAMOD               (*(volatile EVENACCESS uint8_t*)  (BSC_BASE_ADDR + 0x02902u))
#define SDIR                 (*(volatile EVENACCESS uint16_t*) (BSC_BASE_ADDR + 0x02924u))
#define SDICR                (*(volatile EVENACCESS uint8_t*)  (BSC_BASE_ADDR + 0x02920u))
#define SDSR                 (*(volatile EVENACCESS uint8_t*)  (BSC_BASE_ADDR + 0x02950u))
#define SDCMOD               (*(volatile EVENACCESS uint8_t*)  (BSC_BASE_ADDR + 0x02901u))
#define SDCCR                (*(volatile EVENACCESS uint8_t*)  (BSC_BASE_ADDR + 0x02900u))
#define SDMOD                (*(volatile EVENACCESS uint16_t*) (BSC_BASE_ADDR + 0x02948u))
#define SDRFCR               (*(volatile EVENACCESS uint16_t*) (BSC_BASE_ADDR + 0x02914u))
#define SDTR                 (*(volatile EVENACCESS uint32_t*) (BSC_BASE_ADDR + 0x02944u))
#define SDADR                (*(volatile EVENACCESS uint8_t*)  (BSC_BASE_ADDR + 0x02940u))
#define SDRFEN               (*(volatile EVENACCESS uint8_t*)  (BSC_BASE_ADDR + 0x02916u))

#define PORTA_DSCR           (*(volatile EVENACCESS uint8_t*)  (0x00008C0EAu))
#define PORTB_DSCR           (*(volatile EVENACCESS uint8_t*)  (0x00008C0EBu))
#define PORTD_DSCR           (*(volatile EVENACCESS uint8_t*)  (0x00008C0EDu))
#define PORTE_DSCR           (*(volatile EVENACCESS uint8_t*)  (0x00008C0EEu))




#define SCI7_BASE_ADDR       (0x0008A0E0u)

#define PWPR                 (*(volatile EVENACCESS uint8_t*)(0x0008C11Fu))
#define P90PFS               (*(volatile EVENACCESS uint8_t*)(0x0008C188u))
#define P92PFS               (*(volatile EVENACCESS uint8_t*)(0x0008C18Au))
#define P9PMR                (*(volatile EVENACCESS uint8_t*)(0x0008C069u))


#define OS_UART_MSTPCR     (MSTPCRB)
#define OS_UART_MSTPCR_BIT (24u)
#define OS_UART_BASE_ADDR  (SCI7_BASE_ADDR)
#define OS_UART_IPR_RX     (IPR98)
#define OS_UART_IPR_TX     (IPR99)
#define OS_UART_IER_RX     (IER0C)
#define OS_UART_IER_TX     (IER0C)
#define OS_UART_IER_RX_BIT (2u)
#define OS_UART_IER_TX_BIT (3u)
#define OS_UART_RX_VECT     98u
#define OS_UART_TX_VECT     99u


#define OS_UART_INT_PRIO     (0x01u)     // Lowest priority

#define OS_UART_SMR          (*(volatile EVENACCESS uint8_t*) (OS_UART_BASE_ADDR + 0x00u))
#define OS_UART_BRR          (*(volatile EVENACCESS uint8_t*) (OS_UART_BASE_ADDR + 0x01u))
#define OS_UART_SCR          (*(volatile EVENACCESS uint8_t*) (OS_UART_BASE_ADDR + 0x02u))
#define OS_UART_TDR          (*(volatile EVENACCESS uint8_t*) (OS_UART_BASE_ADDR + 0x03u))
#define OS_UART_SSR          (*(volatile EVENACCESS uint8_t*) (OS_UART_BASE_ADDR + 0x04u))
#define OS_UART_RDR          (*(volatile EVENACCESS uint8_t*) (OS_UART_BASE_ADDR + 0x05u))
#define OS_UART_SCMR         (*(volatile EVENACCESS uint8_t*) (OS_UART_BASE_ADDR + 0x06u))

#define UART_ERR_PER_BIT     (3u)
#define UART_ERR_FER_BIT     (4u)
#define UART_ERR_ORER_BIT    (5u)
#define UART_ERR_PER_MASK    (1u << UART_ERR_PER_BIT)
#define UART_ERR_FER_MASK    (1u << UART_ERR_FER_BIT)
#define UART_ERR_ORER_MASK   (1u << UART_ERR_ORER_BIT)
#define UART_ERR_MASK_ALL    (UART_ERR_PER_MASK | UART_ERR_FER_MASK | UART_ERR_ORER_MASK)

#ifndef   OS_BAUDRATE
#define OS_BAUDRATE (115200u)
#endif
#define OS_FSYS 120000000u        /* Assume 120MHz*/

#ifndef   OS_PCLK_UART
#define OS_PCLK_UART  (OS_FSYS/2u)
#endif

#define OS_BAUDDIVIDE (((OS_PCLK_UART + (OS_BAUDRATE * 16u)) / (OS_BAUDRATE * 32u)) - 1u)
/****************************************************************************************/

#pragma interrupt (_OS_ISR_Rx(vect=OS_UART_RX_VECT)) // Rx data interrupt
static void _OS_ISR_Rx(void)
{
  trace_rxInterrupt(OS_UART_RDR);

}


#pragma interrupt (_OS_ISR_Tx(vect=OS_UART_TX_VECT))  // TX data empty interrupt
static void _OS_ISR_Tx(void)
{
  trace_txInterrupt();
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


  PRCR = 0xA50Bu; // Protect off

  volatile unsigned int i;
  //
  // Enable UART Module
  //
  OS_UART_MSTPCR &= ~(1u << OS_UART_MSTPCR_BIT);
  //
  // Set uart port pins
  //
  PWPR = 0x00u; // Disable write protect
  PWPR = 0x40u; // Enable write access
  P90PFS = 0x0Au; // Set pin function register to uart function
  P92PFS = 0x0Au; // Set pin function register to uart function
  PWPR = 0x80u; // Enable write protect
  P9PMR |= 0x05u; // Set pin to peripheral function
  //
  // Reset, disable Reception and transmission
  //
  OS_UART_SCR = 0x00u;
  //
  // Set transmission mode
  //
  OS_UART_SMR = 0x00u;
  //
  // Set transfer direction, LSB first, 8 bit data length
  //
  OS_UART_SCMR = 0x10u;
  //
  // Set baudrate
  //
  OS_UART_BRR = (uint8_t) (OS_BAUDDIVIDE);
  //
  // Wait for SCI Settling time (1 bit cycle)
  //
  for (i = 0u; i < 3000u; i++)
  {
  }
  //
  // Clear status
  //
  OS_UART_SSR = 0x00u;
  //
  // Setup interrupt control register for UART
  //
  OS_UART_IPR_RX = OS_UART_INT_PRIO; // Lowest priority
  OS_UART_IPR_TX = OS_UART_INT_PRIO; // Lowest priority
  //
  // Enable UART interrupts
  //
  OS_UART_IER_RX |= (1u << OS_UART_IER_RX_BIT); // Enable Rx interrupt
  OS_UART_IER_TX |= (1u << OS_UART_IER_TX_BIT); // Enable Tx empty interrupt
  //
  // Clear interrupt request flags
  //
  *(uint8_t*) (IRR_BASE_ADDR + OS_UART_RX_VECT) = 0u;
  *(uint8_t*) (IRR_BASE_ADDR + OS_UART_TX_VECT) = 0u;
  //
  // Finally enable Rx and Tx interrupts and enable uart Rx and Tx
  //
  OS_UART_SCR |= (1u << 7u) // Tx interrupt enable
  | (1u << 6u); // Rx interrupt enable
  OS_UART_SCR |= (1u << 5u) // Tx enable
  | (1u << 4u); // Rx enable

  PRCR = 0xA500u; // Protect on

}

/**
 * Prueft, ob Daten mit einem UART versendet werden koennen.
 *
 * \result Die Anzahl der Bytes, die gesendet werden koennen.
 */
static uint32_t trace_writePossible(void)
{
  return g_uartData.transmitSize - g_uartData.bytesInTransmitBuffer;
}

/**
 * \brief Schreibt Daten in einen UART Buffer und startet ggf. das Senden der Daten.
 *
 * \param uartData Die Datenstruktur des entsprechenden UARTS.
 * \param buffer Der Datenpuffer.
 * \param bufferSize Die Groesse von bufferSize.
 * \result Die Anzahl der Bytes, die geschrieben werden konnten.
 */
static uint32_t trace_writeInternal(const uint8_t buffer[], const uint32_t bufferSize)
{
  uint32_t writtenBytes = 0U;
  uint32_t bufferIndex = 0U;

  /*Ermittle die Anzahl der Bytes die max. in den Sendespeicher geschrieben werden koennen.*/
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

    /*Deaktiviere den TXE Interrupt*/
    OS_UART_IER_TX &= ~(1u << OS_UART_IER_TX_BIT);

    if (!g_uartData.sendingActive)
    {/*Momentan ist kein Senden aktiv (Transmit-Interrupt war nicht akiviert).*/

      /*Sende das erste Byte.*/
      OS_UART_TDR = buffer[0];
      bufferIndex = 1;
      g_uartData.sendingActive = TRUE;

    }

    /*Schreibe die zu sendenden Daten in den Sendespeicher.*/
    for (; bufferIndex < writtenBytes; bufferIndex++)
    {
      g_uartData.transmit[g_uartData.transmitHead] = buffer[bufferIndex];

      /*Erhoehe den Kopf des Sendespeichers um 1.*/
      g_uartData.transmitHead++;
      if (g_uartData.transmitHead >= g_uartData.transmitSize)
      {
        g_uartData.transmitHead = 0U;
      }

    }

    g_uartData.bytesInTransmitBuffer += writtenBytes;

    /*Aktiviere den TXE Interrupt*/
    OS_UART_IER_TX |= (1u << OS_UART_IER_TX_BIT);

  }/*if (writtenBytes != 0U)*/

  return writtenBytes;
}

/**********************************************************************************************/

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

/**********************************************************************************************/

uint32_t trace_peak(void)
{
  return g_uartData.bytesInReceiveBuffer;
}

/**********************************************************************************************/

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

  /*Deaktiviere den Receive Interrupt.*/
  OS_UART_IER_RX &= ~(1u << OS_UART_IER_RX_BIT);

  if (0U != read_bytes)
  {
    for (uint32_t i = 0U; i < read_bytes; i++)
    {
      buffer[i] = g_uartData.receive[g_uartData.receiveTail];

      /*Erhoehe das Ende des Lesespeichers um 1.*/
      g_uartData.receiveTail++;
      if (g_uartData.receiveTail >= g_uartData.receiveSize)
      {
        g_uartData.receiveTail = 0U;
      }
    }

    g_uartData.bytesInReceiveBuffer -= read_bytes;

  }

  /*Aktiviere den Receive Interrupt.*/
  OS_UART_IER_RX |= (1u << OS_UART_IER_RX_BIT);

  return read_bytes;
}

