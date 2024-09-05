/*!
 * \file drv_cacheApi.c
 * \brief Contains the UART driver.
 * \author Stefan Zieker
 */

#include <drv_uartApi.h>
#include <app_global.h>
#include <app_ringBuffer.h>

#include "stm32l5xx_hal.h"
#include "stm32l5xx_ll_gpio.h"


/*Size of TX buffer .*/
#define PLM_UART_TX_SIZE 16000

/*Size of RX buffer .*/
#define PLM_UART_RX_SIZE 8000

/*The UART instance*/
UART_HandleTypeDef plmUart_instance;

/*The memory used for the RX buffer.*/
static uint8_t plmUart_rxMemory[PLM_UART_RX_SIZE];

/*Buffer used for reception.*/
ringBuf_Object plmUart_rxBuffer;

/*The memory used for the TX buffer.*/
static uint8_t plmUart_txMemory[PLM_UART_TX_SIZE];

/*Buffer used for transmission.*/
ringBuf_Object plmUart_txBuffer;

/*TRUE if the error callback was called.*/
static bool_t plmUart_errorOccured = FALSE;

/*************************************************************************/

void HAL_UART_ErrorCallback(UART_HandleTypeDef *plmUart_instance)
{
  plmUart_errorOccured = TRUE;
}

/**
 * This ISR is called when a bytes is received.
 */
static void drv_uartRxIsr(UART_HandleTypeDef *instance)
{
  uint8_t uhdata;

  /* Check that a Rx process is ongoing */
  if (instance->RxState == HAL_UART_STATE_BUSY_RX)
  {
    uhdata = (uint8_t) READ_REG(instance->Instance->RDR);

    ringBuf_putData(&plmUart_rxBuffer, &uhdata, 1, NULL, NULL);
  }
  else
  {
    /* Clear RXNE interrupt flag */
    __HAL_UART_SEND_REQ(instance, UART_RXDATA_FLUSH_REQUEST);
  }
}

/**
 * This ISR is called when a bytes has been sent.
 */
static void drv_uart_TxIsr(UART_HandleTypeDef *huart)
{
  uint8_t data;

  if (ringBuf_getCount(&plmUart_txBuffer) == 0U)
  {
    /*Disable the UART Transmit Data Register Empty Interrupt */
    CLEAR_BIT(huart->Instance->CR1, USART_CR1_TXEIE_TXFNFIE);
  }
  else
  {
    ringBuf_getData(&plmUart_txBuffer, &data, 1, NULL, NULL);

    huart->Instance->TDR = data;
  }

}

/*************************************************************************/

void LPUART1_IRQHandler(void)
{
  HAL_UART_IRQHandler(&plmUart_instance);
}

/*************************************************************************/

void drv_uartDeinit(void)
{
  HAL_NVIC_DisableIRQ(USART3_IRQn);
  HAL_UART_DeInit(&plmUart_instance);
  HAL_GPIO_DeInit(GPIOD, GPIO_PIN_8 | GPIO_PIN_9);
}

/*************************************************************************/

void drv_uartInit(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = { 0 };
  RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };

  ringBuf_construct(&plmUart_txBuffer, plmUart_txMemory, PLM_UART_TX_SIZE);
  ringBuf_construct(&plmUart_rxBuffer, plmUart_rxMemory, PLM_UART_RX_SIZE);

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_LPUART1;
  PeriphClkInit.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    fault_reportError(FAULT_ERROR_UART_INIT_1);
  }

  /* Peripheral clock enable */
  __HAL_RCC_LPUART1_CLK_ENABLE();

  __HAL_RCC_GPIOG_CLK_ENABLE();
  HAL_PWREx_EnableVddIO2();
  /**LPUART1 GPIO Configuration
   PG7     ------> LPUART1_TX
   PG8     ------> LPUART1_RX
   */
  GPIO_InitStruct.Pin = GPIO_PIN_7 | GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF8_LPUART1;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /* USART1 interrupt Init */
  HAL_NVIC_SetPriority(LPUART1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(LPUART1_IRQn);

  plmUart_instance.Instance = LPUART1;
  plmUart_instance.Init.BaudRate = 115200;
  plmUart_instance.Init.WordLength = UART_WORDLENGTH_8B;
  plmUart_instance.Init.StopBits = UART_STOPBITS_1;
  plmUart_instance.Init.Parity = UART_PARITY_NONE;
  plmUart_instance.Init.Mode = UART_MODE_TX_RX;
  plmUart_instance.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  plmUart_instance.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  plmUart_instance.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  plmUart_instance.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  plmUart_instance.FifoMode = UART_FIFOMODE_DISABLE;

  //Disable the overrun interrupt.
  plmUart_instance.Instance->CR3 |= 1 << USART_CR3_OVRDIS_Pos;
  if (HAL_UART_Init(&plmUart_instance) != HAL_OK)
  {
    fault_reportError(FAULT_ERROR_UART_INIT_2);
  }

  if (HAL_UART_Receive_IT(&plmUart_instance, (uint8_t*) plmUart_rxBuffer.buffer, 1) != HAL_OK)
  {
    fault_reportError(FAULT_ERROR_UART_INIT_3);
  }
  plmUart_instance.RxISR = drv_uartRxIsr;

}

/**
 * Disables the RX interrupt.
 */
static void drv_uartDisableRxIsr(void)
{
  CLEAR_BIT(plmUart_instance.Instance->CR1, (USART_CR1_RXNEIE_RXFNEIE));
}

/**
 * Enables the RX interrupt.
 */
static void drv_uartEnableRxIsr(void)
{
  SET_BIT(plmUart_instance.Instance->CR1, (USART_CR1_RXNEIE_RXFNEIE));
}

/*************************************************************************/

uint32_t drv_uartGetData(uint8_t *buffer, uint32_t size)
{
  uint32_t numberOfBytes = 0;

  if (plmUart_errorOccured)
  {
    /*Reenable the reception with interrupts.*/
    if (HAL_UART_Receive_IT(&plmUart_instance, (uint8_t*) plmUart_rxBuffer.buffer, 1) != HAL_OK)
    {
      fault_reportError(FAULT_ERROR_UART_GET_DATA);
    }
    plmUart_instance.RxISR = drv_uartRxIsr;
    plmUart_errorOccured = FALSE;
    lua_writestringerror("UART error occured", sizeof("UART error occured") - 1);
  }

  if (ringBuf_getCount(&plmUart_rxBuffer))
  {
    numberOfBytes = ringBuf_getData(&plmUart_rxBuffer, buffer, size, drv_uartDisableRxIsr, drv_uartEnableRxIsr);
  }

  return numberOfBytes;
}

/**
 * Disables the TX interrupt.
 */
static void drv_uartDisableTxIsr(void)
{
  /*Disable the UART Transmit Data Register Empty Interrupt */
  CLEAR_BIT(plmUart_instance.Instance->CR1, (USART_CR1_TXEIE_TXFNFIE));
}

/**
 * Enables the TX interrupt.
 */
static void drv_uartEnableTxIsr(void)
{
  /*Enable the UART Transmit Data Register Empty Interrupt */
  SET_BIT(plmUart_instance.Instance->CR1, USART_CR1_TXEIE_TXFNFIE);
}

/*************************************************************************/

uint32_t drv_uartSendData(uint8_t *buffer, uint32_t size)
{
  uint32_t bytesSend = 0;

  plmUart_instance.gState = HAL_UART_STATE_BUSY_TX;
  plmUart_instance.TxISR = drv_uart_TxIsr;

  do
  {
    bytesSend += ringBuf_putData(&plmUart_txBuffer, &buffer[bytesSend], size - bytesSend, drv_uartDisableTxIsr, drv_uartEnableTxIsr);

  }while(bytesSend < size);

  return bytesSend;
}

