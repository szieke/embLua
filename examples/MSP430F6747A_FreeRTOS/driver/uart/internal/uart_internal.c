/**
 * Includes the internal functions and defines of the uart driver.
 */

#include "Uart_internal.h"

/**********************************************************************************************/

uint16_t uart_peakInternal(CONST uartData_t* CONST uartData, bool_t enableReceiveInterrupt)
{
  return uartData->bytesInReceiveBuffer;
}

/**********************************************************************************************/

uint16_t uart_readInternal(uartData_t* CONST uartData, uint8_t buffer[], CONST uint16_t bufferSize)
{
  uint16_t read_bytes = 0U;
  uint16_t i = 0U;

  if (uartData->bytesInReceiveBuffer >= bufferSize)
  {
    read_bytes = bufferSize;
  }
  else
  {
    read_bytes = uartData->bytesInReceiveBuffer;
  }

  /*Disable the receive interrupt.*/
  EUSCI_A_UART_disableInterrupt(uartData->uart, EUSCI_A_UART_RECEIVE_INTERRUPT);

  if (0U != read_bytes)
  {
    for (; i < read_bytes; i++)
    {
      buffer[i] = uartData->receive[uartData->receiveTail];

      uartData->receiveTail++;
      if (uartData->receiveTail >= uartData->receiveSize)
      {
        uartData->receiveTail = 0U;
      }
    }

    uartData->bytesInReceiveBuffer -= read_bytes;

  }

  /*Enable the receive interrupt.*/
  EUSCI_A_UART_enableInterrupt(uartData->uart, EUSCI_A_UART_RECEIVE_INTERRUPT);

  return read_bytes;

}

/**********************************************************************************************/

void uart_irqHandlerInternal(uartData_t* CONST uartData)
{
  if ((HWREG8(uartData->uart + OFS_UCAxIFG) & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG))
  {/*The receive interrupt ist activ.*/

    if (uartData->bytesInReceiveBuffer != uartData->receiveSize)
    {/*The receive buffer if not full.*/

      /*Read the last received byte.*/
      uartData->receive[uartData->receiveHead] = HWREG8(uartData->uart + OFS_UCAxRXBUF);
      uartData->bytesInReceiveBuffer++;

      uartData->receiveHead++;
      if (uartData->receiveHead >= uartData->receiveSize)
      {
        uartData->receiveHead = 0U;
      }

      if (uartData->bytesInReceiveBuffer == uartData->receiveSize)
      {/*The receive buffer if full.*/
      }
    }
    else
    {
      EUSCI_A_UART_clearInterrupt(uartData->uart, EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG);
    }
  }

  if (uartData->transmitIsInProgress
      && (HWREG8(uartData->uart + OFS_UCAxIFG) & EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG))
  {/*Transmit Interrupt.*/

    uartData->bytesInTransmitBuffer--;

    if (uartData->bytesInTransmitBuffer != 0)
    {
      HWREG8(uartData->uart + OFS_UCAxTXBUF) = uartData->transmit[uartData->transmitTail];

      uartData->transmitTail++;
      if (uartData->transmitTail >= uartData->transmitSize)
      {
        uartData->transmitTail = 0U;
      }

    }
    else
    {/*The last byte has been sent.*/

      uartData->transmitIsInProgress = FALSE;

      /*Disable the transmit interrupt.*/
      EUSCI_A_UART_disableInterrupt(uartData->uart, EUSCI_A_UART_TRANSMIT_INTERRUPT);
    }
  }
}

/**********************************************************************************************/

uint16_t uart_writePossibleInternal(CONST uartData_t* CONST uartData)
{
  uint16_t freeSpace = 0;

  if(uartData->transmitIsInProgress)
  {
    /*Disable the transmit interrupt.*/
    EUSCI_A_UART_disableInterrupt(uartData->uart, EUSCI_A_UART_TRANSMIT_INTERRUPT);
  }

  freeSpace = uartData->transmitSize - uartData->bytesInTransmitBuffer;

  if (uartData->transmitIsInProgress)
  {
    EUSCI_A_UART_enableInterrupt(uartData->uart, EUSCI_A_UART_TRANSMIT_INTERRUPT);
  }

  return freeSpace;
}

/**********************************************************************************************/

uint16_t uart_writeInternal(uartData_t* CONST uartData, CONST uint8_t buffer[], CONST uint16_t bufferSize)
{
  uint16_t writtenBytes = 0U;
  uint16_t bufferIndex = 0U;

  /*Get the number of bytes which can be written to the transmit buffer.*/
  uint16_t freeSpace = uart_writePossibleInternal(uartData);

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

    /*Disable transmit Interrupt.*/
    EUSCI_A_UART_disableInterrupt(uartData->uart, EUSCI_A_UART_TRANSMIT_INTERRUPT);

    if(!uartData->transmitIsInProgress)
    {/*No transmitting is in progress*/

      /*Send the first byte.*/
      HWREG8(uartData->uart + OFS_UCAxTXBUF) = buffer[0];
      bufferIndex = 1;
      uartData->transmitIsInProgress = TRUE;

    }

    /*Write the data to the transmit buffer.*/
    for (; bufferIndex < writtenBytes; bufferIndex++)
    {
      uartData->transmit[uartData->transmitHead] = buffer[bufferIndex];

      uartData->transmitHead++;
      if (uartData->transmitHead >= uartData->transmitSize)
      {
        uartData->transmitHead = 0U;
      }

    }

    uartData->bytesInTransmitBuffer += writtenBytes;

    /*Enable the transmit Interrupt.*/
    EUSCI_A_UART_enableInterrupt(uartData->uart, EUSCI_A_UART_TRANSMIT_INTERRUPT);

  }/*if (writtenBytes != 0U)*/

  return writtenBytes;
}

