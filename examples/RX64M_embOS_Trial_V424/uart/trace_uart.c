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
  OS_TASK* rxTask; //Task die, die empfangenen Daten auswertet.

  bool_t sendingActive;

} trace_uartData_t;

/*!< Beinhaltet alle UART Datenstrukturen.*/
static trace_uartData_t g_uartData;

#define EVENACCESS             __evenaccess
#define IER0C                (*(volatile EVENACCESS OS_U8*)  (0x0008720Cu))
#define SCI7_BASE_ADDR       (0x0008A0E0u)
#define OS_UART_BASE_ADDR  (SCI7_BASE_ADDR)
#define OS_UART_TDR          (*(volatile EVENACCESS OS_U8*) (OS_UART_BASE_ADDR + 0x03u))
#define OS_UART_IER_RX     (IER0C)
#define OS_UART_IER_TX     (IER0C)
#define OS_UART_IER_RX_BIT (2u)
#define OS_UART_IER_TX_BIT (3u)

void trace_rxInterrupt(OS_U8 data)
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

  if (g_uartData.rxTask)
  {
    OS_SignalEvent(TRACE_RECEIVE_EVENT, g_uartData.rxTask);
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

void trace_registerTask(OS_TASK* rxTask)
{
  g_uartData.rxTask = rxTask;
}

/****************************************************************************************/

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
  g_uartData.rxTask = NULL;

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
        OS_Delay(1);
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

