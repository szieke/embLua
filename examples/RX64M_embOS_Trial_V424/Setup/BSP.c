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
 File    : BSP.c
 Purpose : BSP for RSK+RX64M eval board
 --------  END-OF-HEADER  ---------------------------------------------
 */

#include "BSP.h"

/*********************************************************************
 *
 *       Defines
 *
 **********************************************************************
 */
#ifdef __RX
#define EVENACCESS  __evenaccess
#else
#define EVENACCESS
#endif

#define P0_BASE_ADDR       (0x0008C000u)
#define P0_DDR             (*(volatile EVENACCESS unsigned char*) (P0_BASE_ADDR + 0x00u))
#define P0_DR              (*(volatile EVENACCESS unsigned char*) (P0_BASE_ADDR + 0x20u))

/****** Assign LEDs to Ports ****************************************/
#define LED0_BIT           (3u)      // LED 0 on RSK+RX64M
#define LED1_BIT           (5u)      // LED 1 on RSK+RX64M
#define LED0_MASK          (1u << LED0_BIT)
#define LED1_MASK          (1u << LED1_BIT)
#define LED_MASK_ALL       (LED0_MASK | LED1_MASK)

#define LED_PORT_DATA      P0_DR
#define LED_PORT_DIRECTION P0_DDR

/*********************************************************************
 *
 *       Global functions
 *
 **********************************************************************
 */

/*********************************************************************
 *
 *       BSP_Init()
 */
void BSP_Init(void)
{
  LED_PORT_DATA |= LED_MASK_ALL; // Switch off leds initially (active low)
  LED_PORT_DIRECTION |= LED_MASK_ALL; // Switch LED-Port to output
}

/*********************************************************************
 *
 *       BSP_SetLED()
 */
void BSP_SetLED(int Index)
{
  if (Index == 0)
  {
    LED_PORT_DATA &= ~(LED0_MASK); // Switch LED on, active low
  }
  else if (Index == 1)
  {
    LED_PORT_DATA &= ~(LED1_MASK); // Switch LED on, active low
  }
}

/*********************************************************************
 *
 *       BSP_ClrLED()
 */
void BSP_ClrLED(int Index)
{
  if (Index == 0)
  {
    LED_PORT_DATA |= (LED0_MASK); // Switch LED off, set port high
  }
  else if (Index == 1)
  {
    LED_PORT_DATA |= (LED1_MASK); // Switch LED off, set port high
  }
}

/*********************************************************************
 *
 *       BSP_ToggleLED()
 */
void BSP_ToggleLED(int Index)
{
  if (Index == 0)
  {
    LED_PORT_DATA ^= LED0_MASK;
  }
  else if (Index == 1)
  {
    LED_PORT_DATA ^= LED1_MASK;
  }
}

/****** End Of File *************************************************/
