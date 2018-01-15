
#ifndef _HW_INIT_H_
#define _HW_INIT_H_

#include "global.h"

/*!< Debug UART.*/
#define HW_UART_DEBUG_UART EUSCI_A2_BASE

/*!< Debug UART ISR.*/
#define HW_UART_DEBUG_ISR USCI_A2_VECTOR

/*<! Debug UART port.*/
#define HW_UART_PORT_DEBUG GPIO_PORT_P3

/*<! Debug UART Tx pin.*/
#define HW_UART_PIN_DEBUG_TX GPIO_PIN7

/*<! Debug UART Rx pin.*/
#define HW_UART_PIN_DEBUG_RX GPIO_PIN6


/*!< XT1 frequency.*/
#define XT1_CLK 32768

/*!< XT2 frequency.*/
#define XT2_CLK 0


/*!< MCLK (and SMCLK) frequency.*/
#define UCS_MCLK_DESIRED_FREQUENCY_IN_KHZ   25000

void initializeMainComponents(void);


#endif
