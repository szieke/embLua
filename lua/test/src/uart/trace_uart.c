#include "trace_uart.h"

typedef struct
{
  uint8_t* receive;/*!< Receive buffer (ring buffer).*/
  uint32_t receiveSize;/*!< The size of recive.*/
  uint8_t* transmit; /*!< The transmit buffer (ring buffer).*/
  uint32_t transmitSize;/*!< The size of transmit.*/
  uint32_t bytesInReceiveBuffer; /*!< The number of byte in the receive buffer.*/
  uint32_t receiveHead; /*!< The head of the receive buffer.*/
  uint32_t receiveTail; /*!< The tail of the receive buffer.*/
  uint32_t transmitHead; /*!< The head of the transmit buffer.*/
  uint32_t transmitTail; /*!< The tail of the transmit buffer.*/
  uint32_t bytesInTransmitBuffer; /*!< The number of byte in the transmit buffer.*/

  bool_t sendingActive;

} trace_uartData_t;

static trace_uartData_t g_uartData;

#define EVENACCESS             __evenaccess
#define IER0C                (*(volatile EVENACCESS uint8_t*)  (0x0008720Cu))
#define SCI7_BASE_ADDR       (0x0008A0E0u)
#define UART_BASE_ADDR  (SCI7_BASE_ADDR)
#define UART_TDR          (*(volatile EVENACCESS uint8_t*) (UART_BASE_ADDR + 0x03u))
#define UART_IER_RX     (IER0C)
#define UART_IER_TX     (IER0C)
#define UART_IER_RX_BIT (2u)
#define UART_IER_TX_BIT (3u)

/****** Protect register  ********************************************/
#define PRCR                 (*(volatile EVENACCESS uint16_t*) (0x000803FEu))

/****** Interrupt controller       **********************************/
#define IRR_BASE_ADDR        (0x00087000u)

#define IER0C                (*(volatile EVENACCESS uint8_t*)  (0x0008720Cu))
#define IPR98                (*(volatile EVENACCESS uint8_t*)  (0x00087362u))
#define IPR99                (*(volatile EVENACCESS uint8_t*)  (0x00087363u))

/****** SYSTEM module  **********************************************/
#define SYSTEM_BASE_ADDR     (0x00080000u)
#define MSTPCRB              (*(volatile EVENACCESS uint32_t*) (SYSTEM_BASE_ADDR + 0x014u))

#define SCI7_BASE_ADDR       (0x0008A0E0u)

#define PWPR                 (*(volatile EVENACCESS uint8_t*)(0x0008C11Fu))
#define P90PFS               (*(volatile EVENACCESS uint8_t*)(0x0008C188u))
#define P92PFS               (*(volatile EVENACCESS uint8_t*)(0x0008C18Au))
#define P9PMR                (*(volatile EVENACCESS uint8_t*)(0x0008C069u))

#define UART_MSTPCR     (MSTPCRB)
#define UART_MSTPCR_BIT (24u)
#define UART_BASE_ADDR  (SCI7_BASE_ADDR)
#define UART_IPR_RX     (IPR98)
#define UART_IPR_TX     (IPR99)
#define UART_IER_RX     (IER0C)
#define UART_IER_TX     (IER0C)
#define UART_IER_RX_BIT (2u)
#define UART_IER_TX_BIT (3u)
#define UART_RX_VECT     98u
#define UART_TX_VECT     99u

#define UART_INT_PRIO     (0x01u)     // Lowest priority

#define UART_SMR          (*(volatile EVENACCESS uint8_t*) (UART_BASE_ADDR + 0x00u))
#define UART_BRR          (*(volatile EVENACCESS uint8_t*) (UART_BASE_ADDR + 0x01u))
#define UART_SCR          (*(volatile EVENACCESS uint8_t*) (UART_BASE_ADDR + 0x02u))
#define UART_TDR          (*(volatile EVENACCESS uint8_t*) (UART_BASE_ADDR + 0x03u))
#define UART_SSR          (*(volatile EVENACCESS uint8_t*) (UART_BASE_ADDR + 0x04u))
#define UART_RDR          (*(volatile EVENACCESS uint8_t*) (UART_BASE_ADDR + 0x05u))
#define UART_SCMR         (*(volatile EVENACCESS uint8_t*) (UART_BASE_ADDR + 0x06u))

#define UART_ERR_PER_BIT     (3u)
#define UART_ERR_FER_BIT     (4u)
#define UART_ERR_ORER_BIT    (5u)
#define UART_ERR_PER_MASK    (1u << UART_ERR_PER_BIT)
#define UART_ERR_FER_MASK    (1u << UART_ERR_FER_BIT)
#define UART_ERR_ORER_MASK   (1u << UART_ERR_ORER_BIT)
#define UART_ERR_MASK_ALL    (UART_ERR_PER_MASK | UART_ERR_FER_MASK | UART_ERR_ORER_MASK)

#ifndef   BAUDRATE
#define BAUDRATE (115200u)
#endif
#define FSYS 120000000u        /* Assume 120MHz*/

#ifndef   PCLK_UART
#define PCLK_UART  (FSYS/2u)
#endif

#define BAUDDIVIDE (((PCLK_UART + (BAUDRATE * 16u)) / (BAUDRATE * 32u)) - 1u)

/****************************************************************************************/

#pragma interrupt (_ISR_Rx(vect=UART_RX_VECT)) // Rx data interrupt
static void _ISR_Rx(void)
{
  if (g_uartData.bytesInReceiveBuffer != g_uartData.receiveSize)
  {/*The receive buffer is not full.*/

    g_uartData.receive[g_uartData.receiveHead] = UART_RDR;
    g_uartData.bytesInReceiveBuffer++;

    g_uartData.receiveHead++;
    if (g_uartData.receiveHead >= g_uartData.receiveSize)
    {
      g_uartData.receiveHead = 0U;
    }

    if (g_uartData.bytesInReceiveBuffer == g_uartData.receiveSize)
    {/*The receive buffer is not full*/
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
      UART_TDR = g_uartData.transmit[g_uartData.transmitTail];

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
  UART_MSTPCR &= ~(1u << UART_MSTPCR_BIT);
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
  UART_SCR = 0x00u;
  //
  // Set transmission mode
  //
  UART_SMR = 0x00u;
  //
  // Set transfer direction, LSB first, 8 bit data length
  //
  UART_SCMR = 0x10u;
  //
  // Set baudrate
  //
  UART_BRR = (uint8_t) (BAUDDIVIDE);
  //
  // Wait for SCI Settling time (1 bit cycle)
  //
  for (i = 0u; i < 3000u; i++)
  {
  }
  //
  // Clear status
  //
  UART_SSR = 0x00u;
  //
  // Setup interrupt control register for UART
  //
  UART_IPR_RX = UART_INT_PRIO; // Lowest priority
  UART_IPR_TX = UART_INT_PRIO; // Lowest priority
  //
  // Enable UART interrupts
  //
  UART_IER_RX |= (1u << UART_IER_RX_BIT); // Enable Rx interrupt
  UART_IER_TX |= (1u << UART_IER_TX_BIT); // Enable Tx empty interrupt
  //
  // Clear interrupt request flags
  //
  *(uint8_t*) (IRR_BASE_ADDR + UART_RX_VECT) = 0u;
  *(uint8_t*) (IRR_BASE_ADDR + UART_TX_VECT) = 0u;
  //
  // Finally enable Rx and Tx interrupts and enable uart Rx and Tx
  //
  UART_SCR |= (1u << 7u) // Tx interrupt enable
  | (1u << 6u); // Rx interrupt enable
  UART_SCR |= (1u << 5u) // Tx enable
  | (1u << 4u); // Rx enable

  PRCR = 0xA500u; // Protect on

}

static uint32_t trace_writePossible(void)
{
  uint32_t result;

  /*Disable TXE interrupt*/
  UART_IER_TX &= ~(1u << UART_IER_TX_BIT);

  result = g_uartData.transmitSize - g_uartData.bytesInTransmitBuffer;

  if (g_uartData.sendingActive)
  {
    /*Enable TXE interrupt*/
    UART_IER_TX |= (1u << UART_IER_TX_BIT);
  }

  return result;
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

    /*Disable TXE interrupt*/
    UART_IER_TX &= ~(1u << UART_IER_TX_BIT);

    if (!g_uartData.sendingActive)
    {

      UART_TDR = buffer[0];
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

    if (g_uartData.sendingActive)
    {
      /*Enable TXE interrupt*/
      UART_IER_TX |= (1u << UART_IER_TX_BIT);
    }

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

  /*Disable RXE interrupt*/
  UART_IER_RX &= ~(1u << UART_IER_RX_BIT);

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

  /*Enable RXE interrupt*/
  UART_IER_RX |= (1u << UART_IER_RX_BIT);

  return read_bytes;
}

