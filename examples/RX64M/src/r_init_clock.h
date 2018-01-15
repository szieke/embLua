/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2017 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* System Name  : RX64M initialization example
* File Name    : r_init_clock.h
* Version      : Ver 1.00
* Device       : R5F564MLDxFC(RX64M Group)
* Abstract     : Program example of RX64M initialization
* Tool-Chain   : e2 studio Version 5.4.0.015
*              : C/C++ Compiler Package for RX Family (V.2.06.00)
* OS           : not use
* H/W Platform : Renesas Starter Kit for RX64M
* Description  : Initialize the clock.
* Limitation   : none
*******************************************************************************/
/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 31.05.2017 1.00     First Release
*******************************************************************************/

/* Guards against multiple inclusion */
#ifndef R_INIT_CLOCK_H
#define R_INIT_CLOCK_H

#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
Macro definitions
*******************************************************************************/

/* The value to select whether to use */
#define B_NOT_USE       (0)
#define B_USE           (1)
#define B_USE_PLL_MAIN  (2)
#define B_USE_PLL_HOCO  (3)

/* ==== Please set the main clock ==== */
/* This sample code uses the main clock with the following settings.
   Please change the following settings as necessary. */
#define SEL_MAIN        (B_USE)
/* Frequency : 24 MHz */
#define MAIN_CLOCK_Hz   (24000000L)
/* Drive capability : 24 MHz crystal resonator */
#define REG_MOFCR       (0x00)
/* Wait control register : 9.98ms(The maximum of fLOCO(264kHz)) */
#define REG_MOSCWTCR    (0x53)

/* ==== Please set the sub-clock ==== */
/* This sample code does not use the sub-clock.
   Please change the following settings as necessary. */
#define SEL_SUB         (B_NOT_USE)       /* Please select whether to use sub-clock as the system clock */
#define SEL_RTC         (B_NOT_USE)       /* Please select whether to use sub-clock as the RTC count source */
/* Frequency : 32.768 kHz */
#define SUB_CLOCK_Hz    (32768L)
/* Wait control register : 2.044ms (default value) */
#define REG_SOSCWTCR    (0x21)
/* ---- Please select the drive capability ---- */
#define CL_LOW          (0x02)          /* Low CL */
#define CL_STD          (0x0C)          /* Standard CL */
#define REG_RCR3        (CL_LOW)

/* ==== Please set the PLL clock ==== */
/* This sample code uses the PLL clock with the following settings.
   Please change the following settings as necessary. */
#define SEL_PLL         (B_USE_PLL_MAIN)
/* Division ratio and multiplication factor and clock source.
   : no division, multiply-by-10, main clock */
#define REG_PLLCR       (0x1300)

/* ==== Please set the HOCO clock ==== */
/* This sample code does not use the HOCO clock.
   Please change the following settings as necessary. */
#define SEL_HOCO        (B_NOT_USE)
/* ---- Please select the HOCO frequency ---- */
#define FREQ_16MHz      (0x00)          /* 16 MHz */
#define FREQ_18MHz      (0x01)          /* 18 MHz */
#define FREQ_20MHz      (0x02)          /* 20 MHz */
#define REG_HOCOCR2     (FREQ_20MHz)

/* ==== Please select the source of the system clock ==== */

    #define CLK_PLL     (0x0400)        /* PLL */
    #define CLK_HOCO    (0x0100)        /* HOCO */
    #define CLK_SUB     (0x0300)        /* Sub-clock */
    #define CLK_MAIN    (0x0200)        /* Main clock */
/* This sample code uses PLL as the system clock.
   Please change the following settings as necessary. */
#define SEL_SYSCLK      (CLK_PLL)

/* ==== Please select the internal clock division ratio ==== */
/* This sample code uses the following internal clock division ratio.
   Please change the following settings as necessary. */
#if (SEL_SYSCLK == CLK_PLL)
    #define REG_SCKCR   (0x21C21222)    /* ICLK,PCLKA: divide-by-2
                                           PCLKB-D,BCLK,FCLK: divide-by-4 */
#elif (SEL_SYSCLK == CLK_HOCO)
    #define REG_SCKCR   (0x10C10111)    /* ICLK,PCLKA: no division
                                           PCLKB-D,BCLK,FCLK: divide-by-2 */
#else
    #define REG_SCKCR   (0x55C55555)    /* ALL : divide-by-32 */
#endif

/* ==== Please select the operating power control mode ==== */
#define OPCM_HIGH       (0x00)          /* High-speed operating mode */
#if (SEL_PLL == B_NOT_USE)
    #define OPCM_LOW_1  (0x06)          /* Low-speed operating mode 1 */
#endif
#if (SEL_PLL == B_NOT_USE) && (SEL_HOCO == B_NOT_USE)
    #define OPCM_LOW_2  (0x07)          /* Low-speed operating mode 2 */
#endif
/* This sample code uses the High-speed operating mode.
   Please change the following settings as necessary. */
#define REG_OPCCR       (OPCM_HIGH)



/*******************************************************************************
Exported global variables and functions (to be accessed by other files)
*******************************************************************************/
void R_INIT_Clock(void);
void CGC_oscillation_main(void);
void CGC_oscillation_PLL(void);
void CGC_oscillation_HOCO(void);
void CGC_oscillation_sub(void);
void CGC_disable_subclk(void);

#ifdef __cplusplus
  }
#endif

#endif /* R_INIT_CLOCK_H */

/* End of File */

