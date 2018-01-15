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
File    : JLINKRX.h
Purpose : Header file for J-Link RX communication using memory
---------------------------END-OF-HEADER------------------------------
*/

#ifndef JLINKMEM_H
#define JLINKMEM_H             // Avoid multiple inclusion

#ifdef __cplusplus
extern "C" {
#endif

void JLINKRX_Process(void);
void JLINKRX_SetpfOnRx(void (* pf)(unsigned char Data));
void JLINKRX_SetpfOnTx(void (* pf)(void));
void JLINKRX_SetpfGetNextChar(OS_INT (* pf)(void));
void JLINKRX_SendChar(unsigned char Data);

#ifdef __cplusplus
}
#endif

#endif                         // Avoid multiple inclusion

/*************************** end of file ****************************/

