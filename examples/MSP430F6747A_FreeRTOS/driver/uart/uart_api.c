/**
 * Includes the API functions and defines of the uart driver.
 */

#include "uart_api.h"
#include "uart_internal.h"
#include "hwInit.h"



/*!< Contains all UART data structures.*/
STATIC uartData_t g_uartData[UART_END_MARKER];

/*!< Debug-UART receive buffer.*/
uint8_t g_debugReceive[UART_DEBUG_RECEIVE_BUFFER_SIZE];

/*!< Debug-UART transmit buffer.*/
uint8_t g_debugTransmit[UART_DEBUG_TRANSMIT_BUFFER_SIZE];

/**
 * Debug-UART ISR.
 */
#pragma vector=HW_UART_DEBUG_ISR
__interrupt void uart_debugIsr(void)
{
  uart_irqHandlerInternal(&g_uartData[UART_DEBUG]);
}


/**
 * Initializes one UART.
 *
 * \param uart [in] The address of the UART.
 * \param baudrate [in] The desired baudrate.
 */
static void uart_initializeUart(uint16_t uart, uint32_t baudrate)
{
  uint32_t clockValue = UCS_getSMCLK();
  uint32_t tmp;


  EUSCI_A_UART_initParam param = { 0 };
  param.selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK;
  param.clockPrescalar = clockValue / baudrate;
  param.firstModReg = 0;
  param.parity = EUSCI_A_UART_NO_PARITY;
  param.msborLsbFirst = EUSCI_A_UART_LSB_FIRST;
  param.numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT;
  param.uartMode = EUSCI_A_UART_MODE;
  param.overSampling = EUSCI_A_UART_LOW_FREQUENCY_BAUDRATE_GENERATION;

  tmp = ((clockValue % baudrate) * 100);
  tmp = (tmp / baudrate) * 8;
  tmp /= 100;
  param.secondModReg = tmp;

  (void) EUSCI_A_UART_init(uart, &param);

  //Enable the UART.
  EUSCI_A_UART_enable(uart);

  //Enable den Receive Interrupt.
  EUSCI_A_UART_clearInterrupt(uart, EUSCI_A_UART_RECEIVE_INTERRUPT);
  EUSCI_A_UART_enableInterrupt(uart, EUSCI_A_UART_RECEIVE_INTERRUPT);

}

/****************************************************************************************/

void uart_initializeDebug()
{

  g_uartData[UART_DEBUG].receive = g_debugReceive;
  g_uartData[UART_DEBUG].receiveSize = UART_DEBUG_RECEIVE_BUFFER_SIZE;
  g_uartData[UART_DEBUG].transmit = g_debugTransmit;
  g_uartData[UART_DEBUG].transmitSize = UART_DEBUG_TRANSMIT_BUFFER_SIZE;
  g_uartData[UART_DEBUG].receiveTail = 0U;
  g_uartData[UART_DEBUG].receiveHead = 0U;
  g_uartData[UART_DEBUG].transmitTail = 0U;
  g_uartData[UART_DEBUG].transmitHead = 0U;
  g_uartData[UART_DEBUG].uart = HW_UART_DEBUG_UART;
  g_uartData[UART_DEBUG].port = HW_UART_PORT_DEBUG;
  g_uartData[UART_DEBUG].txPin = HW_UART_PIN_DEBUG_TX;
  g_uartData[UART_DEBUG].rxPin = HW_UART_PIN_DEBUG_RX;
  g_uartData[UART_DEBUG].bytesInReceiveBuffer = 0;
  g_uartData[UART_DEBUG].bytesInTransmitBuffer = 0;
  g_uartData[UART_DEBUG].transmitIsInProgress = FALSE;
  g_uartData[UART_DEBUG].apiSemaphore = xSemaphoreCreateRecursiveMutex();

  GPIO_setAsPeripheralModuleFunctionInputPin(g_uartData[UART_DEBUG].port,
      g_uartData[UART_DEBUG].txPin + g_uartData[UART_DEBUG].rxPin);

  uart_initializeUart(g_uartData[UART_DEBUG].uart, UART_DEBUG_BAUDRATE);

}


/**********************************************************************************************/

void uart_setBaudrate(CONST uartInstance_t instance, uint32_t baudrate)
{
  xSemaphoreTakeRecursive(g_uartData[instance].apiSemaphore, portMAX_DELAY);
  uart_initializeUart(g_uartData[instance].uart, baudrate);
  xSemaphoreGive(g_uartData[instance].apiSemaphore);
}

/****************************************************************************************/

void uart_blockUntilAllSent(CONST uartInstance_t instance)
{

  xSemaphoreTakeRecursive(g_uartData[instance].apiSemaphore, portMAX_DELAY);

  //Block until all bytes have been sent.
  while (uart_writePossible(instance) != g_uartData[instance].transmitSize)
  {
    _nop();

  }

  xSemaphoreGive(g_uartData[instance].apiSemaphore);
}


/**********************************************************************************************/

uint16_t uart_writePossible(CONST uartInstance_t instance)
{
  uint16_t result = 0;

  xSemaphoreTakeRecursive(g_uartData[instance].apiSemaphore, portMAX_DELAY);
  result = uart_writePossibleInternal(&g_uartData[instance]);
  xSemaphoreGive(g_uartData[instance].apiSemaphore);

  return result;
}

/**********************************************************************************************/

uint16_t uart_write(CONST uartInstance_t instance, CONST uint8_t buffer[], CONST uint16_t bufferSize,
    bool_t waitUntilAllBytesAreInBuffer)
{


  uint16_t bytesSend = 0;

  xSemaphoreTakeRecursive(g_uartData[instance].apiSemaphore, portMAX_DELAY);

  if (waitUntilAllBytesAreInBuffer)
  {
    do
    {
      bytesSend += uart_writeInternal(&g_uartData[instance], &buffer[bytesSend], bufferSize - bytesSend);
      if ((bytesSend < bufferSize))
      {
        _nop();
      }
    }
    while (bytesSend < bufferSize);
  }
  else
  {
    bytesSend = uart_writeInternal(&g_uartData[instance], buffer, bufferSize);
  }

  xSemaphoreGive(g_uartData[instance].apiSemaphore);

  return bytesSend;

}

/**********************************************************************************************/

uint16_t uart_peak(CONST uartInstance_t instance)
{
  xSemaphoreTakeRecursive(g_uartData[instance].apiSemaphore, portMAX_DELAY);

  uint16_t result = uart_peakInternal(&g_uartData[instance], TRUE);

  xSemaphoreGive(g_uartData[instance].apiSemaphore);
  return result;
}

/**********************************************************************************************/

uint16_t uart_read(CONST uartInstance_t instance, uint8_t buffer[], CONST uint16_t bufferSize)
{
  xSemaphoreTakeRecursive(g_uartData[instance].apiSemaphore, portMAX_DELAY);

  uint16_t result = uart_readInternal(&g_uartData[instance], buffer, bufferSize);

  xSemaphoreGive(g_uartData[instance].apiSemaphore);
  return result;
}

