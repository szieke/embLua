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
* File Name    : r_init_clock.c
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

/*******************************************************************************
Includes <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdint.h>
#include "iodefine.h"
#include "r_init_clock.h"

/*******************************************************************************
Macro definitions
*******************************************************************************/
#define SUB_CLOCK_CYCLE  (1000000000L/SUB_CLOCK_Hz)

/* Time for one count of CMT0 is approximately 121212 ns
   when the count source is LOCO divided by 32 (Max. of LOCO = 264 kHz) */
#define FOR_CMT0_TIME    (121212L)

/*******************************************************************************
Private variables and functions
*******************************************************************************/
static void oscillation_subclk(void);
static void resetting_wtcr_subclk(void);
static void init_rtc(void);
static void cmt0_wait(uint32_t cnt);


/*******************************************************************************
* Outline      : Clock initialization
* Header       : r_init_clock.h
* Function Name: R_INIT_Clock
* Description  : Initialize the clock.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void R_INIT_Clock (void)
{

    /* ---- Enable write protection ---- */
    /* PRCR - Protect Register
    b15:b8  PRKEY - PRC Key Code - A5h
                  (The write value should be A5h to permission writing PRCi bit)
    b7:b4   Reserved - The write value should be 0.
    b3      PRC3 - Protect Bit 3 - Write disabled
    b2      Reserved - The write value should be 0.
    b1      PRC1 - Protect Bit 1 - Write enabled
    b0      PRC0 - Protect Bit 0 - Write enabled */
    SYSTEM.PRCR.WORD = 0xA503;

    /* ---- Turn off the HOCO power supply ---- */
#if (SEL_HOCO == B_NOT_USE)
    /* HOCOPCR - High-Speed On-Chip Oscillator Power Supply Control Register
    b7:b1   Reserved - The write value should be 0.
    b0      HOCOPCNT - High-Speed On-Chip Oscillator Power Supply Control
                     - HOCO power supply is turned off. */
    SYSTEM.HOCOPCR.BYTE = 0x01;

    /* OSCOVFSR - Oscillation Stabilization Flag Register
     b7:b5  Reserved - The write value should be 0.
     b4     ILCOVF - IWDT Low-Speed Clock Oscillation Stabilization Flag
     b3     HCOVF - HOCO Clock Oscillation Stabilization Flag
     b2     PLOVF - PLL Clock Oscillation Stabilization Flag
     b1     SOOVF - Sub-Clock Oscillation Stabilization Flag
     b0     MOOVF - Main Clock Oscillation Stabilization Flag
     */
 #endif

    /* ---- Turn off the PLL power supply ---- */
#if (SEL_PLL == B_NOT_USE)
    /* PLLCR2 - PLL Control Register 2
    b7:b1   Reserved - The write value should be 0.
    b0      PLLEN  - PLL Control Register - Powered off */
    SYSTEM.PLLCR2.BYTE = 0x01;

#endif

    /* ---- Set the sub-clock ---- */
#if ((SEL_SUB == B_USE) || (SEL_RTC == B_USE))
    CGC_oscillation_sub();
#else
    CGC_disable_subclk();
#endif

    /* ---- Set the main clock ---- */
#if ((SEL_MAIN == B_USE) || (SEL_PLL == B_USE_PLL_MAIN))
    CGC_oscillation_main();
#endif

    /* ---- Set the HOCO clock ---- */
#if ((SEL_HOCO == B_USE) || (SEL_PLL == B_USE_PLL_HOCO))
    CGC_oscillation_HOCO();
#endif

    /* ---- Set the PLL clock ---- */
#if ((SEL_PLL == B_USE_PLL_MAIN) || (SEL_PLL == B_USE_PLL_HOCO))
    CGC_oscillation_PLL();
#endif


#if (REG_OPCCR == OPCM_HIGH)
    /* ---- Set the operating power control mode ---- */
    /* OPCCR - Operating Power Control Register
    b7:b5   Reserved - The write value should be 0.
    b4      OPCMTSF  - Operating Power Control Mode Transition Status Flag
    b3      Reserved - The write value should be 0.
    b2:b0   OPCM     - Operating Power Control Mode Select - High-speed operating mode */
    SYSTEM.OPCCR.BYTE = REG_OPCCR;

    while (0 != SYSTEM.OPCCR.BIT.OPCMTSF)
    {
        /* Confirm that the operation power control mode transition completed. */
    }

#endif

    /* ---- Set the internal clock division ratio ---- */
    /* SCKCR - System Clock Control Register
    b31:b28 FCK      - Flash-IF Clock(FCLK) Select - divide-by-4
    b27:b24 ICK      - System Clock (ICLK) Select - divide-by-2
    b23     PSTOP1   - BCLK Pin Output Control    - disabled. (Fixed high)
    b22     PSTOP0   - SDCLK Pin Output Control   - disabled. (Fixed high)
    b21:b20 Reserved - The write value should be 0.
    b19:b16 BCK      - External Bus Clock (BCLK) Select - divide-by-4
    b15:b12 PCLKA    - Peripheral Module Clock A(PCLKB) Select - divide-by-2
    b10:b8  PCLKB    - Peripheral Module Clock B(PCLKB) Select - divide-by-4
    b7:b4   PCLKC    - Peripheral Module Clock C(PCLKB) Select - divide-by-4
    b3:b0   PCLKD    - Peripheral Module Clock D(PCLKD) Select - divide-by-4 */
    SYSTEM.SCKCR.LONG = REG_SCKCR;

    while (REG_SCKCR != SYSTEM.SCKCR.LONG)
    {
         /* Confirm that the written value can be read correctly. */
    }

    /* ---- Set the BCLK pin output ---- */
    /* BCKCR - External Bus Clock Control Register
    b7:b1   Reserved - The write value should be 0.
    b0      BCLKDIV  - BCLK Pin Output Select - divide-by-2 */
    SYSTEM.BCKCR.BYTE = 0x01;

    while (0x01 != SYSTEM.BCKCR.BYTE)
    {
        /* Confirm that the written value can be read correctly. */
    }

    /* ---- Set the internal clock division ratio(USB) ---- */
    /* SCKCR2 - System Clock Control Register 2
    b15:b8 Reserved - The write value should be 0.
    b7:b4  UCK      - USB Clock Select - divide-by-2
    b3:b1  Reserved - The write value should be 0.
    b0     Reserved - The write value should be 1. */
    SYSTEM.SCKCR2.WORD = 0x0011;

    while(0x0011 != SYSTEM.SCKCR2.WORD)
    {
    	/* Confirm that the written value can be read correctly. */
    }

    /* ---- Set the internal clock source ---- */
    /* SCKCR3 - System Clock Control Register 3
    b15:b11 Reserved - The write value should be 0.
    b10:b8  CKSEL    - Clock Source Select - PLL circuit is selected.
    b7:b1   Reserved - The write value should be 0. */
    SYSTEM.SCKCR3.WORD = SEL_SYSCLK;

    while (SEL_SYSCLK != SYSTEM.SCKCR3.WORD)
    {
        /* Confirm that the written value can be read correctly. */
    }

#if (REG_OPCCR > OPCM_HIGH)
    /* ---- Set the operating power control mode ---- */
    /* OPCCR - Operating Power Control Register
    b7:b5   Reserved - The write value should be 0.
    b4      OPCMTSF  - Operating Power Control Mode Transition Status Flag
    b3      Reserved - The write value should be 0.
    b2:b0   OPCM     - Operating Power Control Mode Select - Low-speed operating mode 1 */
    SYSTEM.OPCCR.BYTE = REG_OPCCR;

    while (0 != SYSTEM.OPCCR.BIT.OPCMTSF)
    {
        /* Confirm that the operation power control mode transition completed. */
    }

#endif

    /* ---- Disable write protection ---- */
    /* PRCR - Protect Register
    b15:b8 PRKEY - PRC Key Code - A5h
                  (The write value should be A5h to permission writing PRCi bit)
    b2     Reserved - The write value should be 0.
    b1     PRC1 - Protect Bit 1 - Write disabled
    b0     PRC0 - Protect Bit 0 - Write disabled */
    SYSTEM.PRCR.WORD = 0xA500;

}

/*******************************************************************************
* Outline      : Configure main clock oscillation
* Header       : r_init_clock.h
* Function Name: CGC_oscillation_main
* Description  : Set the drive capability and the MOSCWTCR register,
*                then enable main clock oscillation.
*                Wait for the main clock oscillation stabilization time.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void CGC_oscillation_main (void)
{

    /* ----  Set the main clock oscillator drive capability ---- */
    /* MOFCR - Main Clock Oscillator Forced Oscillation Control Register
    b7       Reserved - The write value should be 0.
    b6       MOSEL    - Main Clock Oscillator Switch - Resonator
    b5:b4    MODRV2   - Main Clock Oscillator Drive Capability 2 Switching - 20.1 MHz to 24.0 MHz
    b3:b1    Reserved - The write value should be 0.
    b0       MOFXIN   - Main Clock Oscillator Forced Oscillation - The write value should be 0. */
    SYSTEM.MOFCR.BYTE = REG_MOFCR;

    /* ---- Set wait time until the main clock oscillator stabilizes ---- */
    /* MOSCWTCR - Main Clock Oscillator Wait Control Register
    b7:b0    MSTS     - Main Clock Oscillator Waiting Time
                      - Wait time is 2635 cycles (approx. 9.98 ms). */
    SYSTEM.MOSCWTCR.BYTE = REG_MOSCWTCR;

    /* ---- Operate the main clock oscillator ---- */
    /* MOSCCR   - Main Clock Oscillator Control Register
    b7:b1    Reserved - The write value should be 0.
    b0       MOSTP    - Main Clock Oscillator Stop - Main clock oscillator is operating. */
    SYSTEM.MOSCCR.BYTE = 0x00;

    while (0x00 != SYSTEM.MOSCCR.BYTE)
    {
        /* Confirm that the written value can be read correctly. */
    }

    /* ---- Wait processing for the clock oscillation stabilization ---- */
    while(1 != SYSTEM.OSCOVFSR.BIT.MOOVF)
    {
    	/* Confirm that the Oscillation of the main clock is stable so the clock is
    	 available for use as the system clock.*/
    }

}

/*******************************************************************************
* Outline      : Configure PLL clock oscillation
* Header       : r_init_clock.h
* Function Name: CGC_oscillation_PLL
* Description  : Set the PLL input frequency division ratio and 
*                frequency multiplication factor, set the PLLWTCR, 
*                then enable PLL clock oscillation.
*                Wait for the PLL clock oscillation stabilization time.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void CGC_oscillation_PLL (void)
{

    /* ---- Set the PLL division ratio and multiplication factor ---- */
    /* PLLCR - PLL Control Register
    b15:b14  Reserved  - The write value should be 0.
    b13:b8   STC       - Frequency Multiplication Factor Select
                       - Frequency multiplication factor is multiply-by-10.
    b7:b5    Reserved  - The write value should be 0.
    b4       PLLSRCSEL - PLL Clock Source Select
                       - Main clock oscillator
    b1:b0    PLIDIV    - PLL Input Frequency Division Ratio Select
                       - PLL input division ratio is no division. */
    SYSTEM.PLLCR.WORD = REG_PLLCR;

    /* ---- Operate the PLL clock oscillator ---- */
    /* PLLCR2 - PLL Control Register 2
    b7:b1    Reserved - The write value should be 0.
    b0       PLLEN    - PLL Stop Control - PLL is operating. */
    SYSTEM.PLLCR2.BYTE = 0x00;

    /* ---- Wait processing for the clock oscillation stabilization ---- */
    while(1 != SYSTEM.OSCOVFSR.BIT.PLOVF)
    {
    	/* Confirm that the Oscillation of the pll clock is stable so the clock is
    	 available for use as the system clock.*/
    }
}

/*******************************************************************************
* Outline      : Configure the HOCO clock oscillation
* Header       : r_init_clock.h
* Function Name: CGC_oscillation_HOCO
* Description  : Set the frequency and the HOCOWTCR2 register,
*                then enable the HOCO.
*                Wait for the HOCO clock oscillation stabilization time.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void CGC_oscillation_HOCO (void)
{

    /* ---- Set the HOCO frequency ---- */
    /* HOCOCR2 - High-Speed On-Chip Oscillator Control Register 2
    b7:b2    Reserved - The write value should be 0.
    b1:b0    HCFRQ    - HOCO Frequency Setting - 20 MHz */
    SYSTEM.HOCOCR2.BYTE = REG_HOCOCR2;

    /* ---- Operate the HOCO clock ---- */
    /* HOCOCR - High-Speed On-Chip Oscillator Control Register
    b7:b1    Reserved   - The write value should be 0.
    b0       HCSTP      - the HOCO is operating. */
    SYSTEM.HOCOCR.BYTE = 0x00;

    /* ---- Wait processing for the clock oscillation stabilization ---- */
    while(1 != SYSTEM.OSCOVFSR.BIT.HCOVF)
    {
    	/* Confirm that the Oscillation of the pll clock is stable so the clock is
    	 available for use as the system clock.*/
    }
}

/*******************************************************************************
* Outline      : Configure the Sub-clock oscillation
* Header       : r_init_clock.h
* Function Name: CGC_oscillation_sub
* Description  : Configure the setting when the sub-clock is used
*                as the system clock or RTC count source.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void CGC_oscillation_sub (void)
{

    /* ---- Setting of the sub-clock oscillation  ---- */
    oscillation_subclk();

    /* ---- Resetting the wait control register ---- */
    resetting_wtcr_subclk();

    /* ---- When using the RTC ---- */
    init_rtc();

}

/*******************************************************************************
* Outline      : Configure the Sub-clock stop
* Header       : r_init_clock.h
* Function Name: CGC_disable_subclk
* Description  : Configure the setting when the sub-clock is not used
*                as the system clock or RTC count source.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void CGC_disable_subclk (void)
{

    uint8_t          i;
    volatile uint8_t dummy;

    /* ---- Stop the sub-clock oscillator ---- */
    /* RCR4 - RTC Control Register 4
    b7:b1    Reserved - The write value should be 0.
    b0       RCKSEL   - Count Source Select - Sub-clock oscillator is selected. */
    RTC.RCR4.BIT.RCKSEL = 0;

    /* dummy read four times */
    for (i = 0; i < 4; i++)
    {
        dummy = RTC.RCR4.BYTE;
    }

    /* ---- Stop the sub-clock oscillator ---- */
    RTC.RCR3.BIT.RTCEN = 0;

    /* dummy read four times */
    for (i = 0; i < 4; i++)
    {
        dummy = RTC.RCR3.BIT.RTCEN;
    }

    while (0 != RTC.RCR3.BIT.RTCEN)
    {
        /* Confirm that the written */
    }

    /* SOSCCR - Sub-Clock Oscillator Control Register
    b7:b1    Reserved - The write value should be 0.
    b0       SOSTP    - Sub-clock oscillator Stop - Sub-clock oscillator is stopped. */
    SYSTEM.SOSCCR.BYTE = 0x01;

    while(0x01 != SYSTEM.SOSCCR.BYTE)
    {
        /* Confirm that the written */
    }

    while (0 != SYSTEM.OSCOVFSR.BIT.SOOVF)
    {
        /* Confirm that the Sub clock stopped. */
    }


}

/*******************************************************************************
* Outline      : Sub-clock oscillation
* Header       : none
* Function Name: oscillation_subclk
* Description  : Configure the sub-clock oscillation setting.
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void oscillation_subclk (void)
{

    uint8_t          i;
    volatile uint8_t dummy;

    /* ---- Stop the sub-clock oscillator ---- */
    /* RCR4 - RTC Control Register 4
     b7:b1    Reserved - The write value should be 0.
     b0       RCKSEL   - Count Source Select - Sub-clock oscillator is selected.
     */
    RTC.RCR4.BIT.RCKSEL = 0;

    /* dummy read four times */
    for (i = 0; i < 4; i++)
    {
        dummy = RTC.RCR4.BYTE;
    }

    /* RCR3 - RTC Control Register 3
       b7:b4    Reserved - The write value should be 0.
       b3:b1    RTCDV    - Sub-clock oscillator Drive Ability Control.
       b0       RTCEN    - Sub-clock oscillator is stopped. */
    RTC.RCR3.BIT.RTCEN = 0;

    /* dummy read four times */
    for (i = 0; i < 4; i++)
    {
        dummy = RTC.RCR3.BYTE;
    }

    while (0 != RTC.RCR3.BIT.RTCEN)
    {
        /* Confirm that the written */
    }

    /* SOSCCR - Sub-Clock Oscillator Control Register
    b7:b1    Reserved - The write value should be 0.
    b0       SOSTP    - Sub-clock oscillator Stop - Sub-clock oscillator is stopped. */
    SYSTEM.SOSCCR.BYTE = 0x01;

    while (0x01 != SYSTEM.SOSCCR.BYTE)
    {
        /* Confirm that the written */
    }

    while (0 != SYSTEM.OSCOVFSR.BIT.SOOVF)
    {
        /* Confirm that the Sub clock stopped. */
    }

    /* ---- Set the drive capacity of the sub-clock oscillator ---- */
    RTC.RCR3.BYTE = REG_RCR3;

    /* dummy read four times */
    for (i = 0; i < 4; i++)
    {
        dummy = RTC.RCR3.BYTE;
    }

    while (REG_RCR3 != RTC.RCR3.BYTE)
    {
        /* Confirm that the written */
    }

    /* ---- Set wait time until the sub-clock oscillator stabilizes ---- */
    /* SOSCWTCR - Sub-Clock Oscillator Wait Control Register
    b7:b5    Reserved - The write value should be 0.
    b4:b0    SSTS - Sub-Clock Oscillator Waiting Time - Waiting time is 2.044ms(the maximum value of fLOCO). */
    SYSTEM.SOSCWTCR.BYTE = REG_SOSCWTCR;

    /* ---- Operate the Sub-clock oscillator ---- */
    SYSTEM.SOSCCR.BYTE = 0x00;

    while (0x00 != SYSTEM.SOSCCR.BYTE)
    {
        /* Confirm that the written */
    }

    while (1 != SYSTEM.OSCOVFSR.BIT.SOOVF)
    {
        /* Confirm that the Oscillation of the sub-clock is stable so the clock is
          available for use as the system clock.*/
    }

}


/*******************************************************************************
* Outline      : Set the wait control register
*                (the sub-clock is used as the RTC count source.)
* Header       : none
* Function Name: resetting_wtcr_subclk
* Description  : Set the wait control register when exiting 
*                from the software standby mode.
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void resetting_wtcr_subclk (void)
{

#if (SEL_RTC == B_USE)


    /* ---- Set wait time until the sub-clock oscillator stabilizes ---- */
    SYSTEM.SOSCWTCR.BYTE = 0x00;

#endif

}

/*******************************************************************************
* Outline      : Initial RTC settings
* Header       : none
* Function Name: init_rtc
* Description  : Make RTC initial settings (clock supply setting and RTC softwarereset)
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void init_rtc (void)
{

#if (SEL_RTC == B_USE)

    uint8_t          i;
    volatile uint8_t dummy;

    /* ---- Operate the sub-clock oscillator ---- */
    RTC.RCR3.BIT.RTCEN = 1;

    /* dummy read four times */
    for (i = 0; i < 4; i++)
    {
         dummy = RTC.RCR3.BIT.RTCEN;
    }

    while (1 != RTC.RCR3.BIT.RTCEN)
    {
         /* Confirm that the written value can be read correctly. */
    }
#endif
    /* ---- Wait for six the sub-clock cycles ---- */
    cmt0_wait(((SUB_CLOCK_CYCLE*6)/FOR_CMT0_TIME)+1);

    /* ---- Stop prescaler and counter ---- */
    /* RCR2 - RTC Control Register 2
    b7  CNTMD - Count Mode Select - The calendar count mode.
    b6  HR24  - Hours Mode - The RTC operates in 24-hour mode.
    b5  AADJP - Automatic Adjustment Period Select - The RADJ.ADJ[5:0] setting value is adjusted from
                                                       the count value of the prescaler every 10 seconds.
    b4  AADJE - Automatic Adjustment Enable - Automatic adjustment is enabled.
    b3  RTCOE - RTCOUT Output Enable - RTCOUT output enabled.
    b2  ADJ30 - 30-Second Adjustment - 30-second adjustment is executed.
    b1  RESET - RTC Software Reset - The prescaler and the target registers for RTC software reset are initialized.
    b0  START - start - Prescaler is stopped. */
    RTC.RCR2.BYTE &= 0x7E;

    while (0 != RTC.RCR2.BIT.START)
    {
        /* Confirm that the written value can be read correctly. */
    }

    /* ---- RTC Software Reset ---- */
    RTC.RCR2.BIT.RESET = 1;

    while (0 != RTC.RCR2.BIT.RESET)
    {
        /* Confirm that the written value can be read correctly. */
    }

#if (SEL_RTC == B_NOT_USE)
    /* ---- An alarm interrupt request is disabled ---- */
    /* RCR1 - RTC Control Register 1
    b7:b4  PES   - Periodic Interrupt Select - These bits specify the period for the periodic interrupt.
    b3     RTCOS - RTCOUT Output Select - RTCOUT outputs 1 Hz.
    b2     PIE   - Periodic Interrupt Enable - A periodic interrupt request is disabled.
    b1     CIE   - Carry Interrupt Enable - A carry interrupt request is disabled.
    b0     AIE   - Alarm Interrupt Enable - An alarm interrupt request is disabled. */
    RTC.RCR1.BIT.AIE = 0;

    /* ---- An carry interrupt request is disabled ---- */
    RTC.RCR1.BIT.CIE = 0;

    /* ---- An periodic interrupt request is disabled ---- */
    RTC.RCR1.BIT.PIE = 0;

    while (0x00 != (RTC.RCR1.BYTE & 0x07))
    {
        /* Confirm that the written value can be read correctly. */
    }
#endif
}

/*******************************************************************************
* Outline      : Software wait with CMT0
* Header       : none
* Function Name: cmt0_wait
* Description  : This function use when writing start waiting to RTC register.
* Arguments    : cnt : Wait for the time specified by an argument multiplied 
*                      by FOR_CMT0_TIME (us).
* Return Value : none
* Note         : This processing assumes that no other interrupts occur during the operation.
*******************************************************************************/
static void cmt0_wait (uint32_t cnt)
{

    uint32_t cmt_int_cnt;
    uint32_t backup_mstp;
    uint16_t backup_prcr;

    backup_mstp = SYSTEM.MSTPCRA.LONG;

    /* Casting the 16bit,because an operation result is 32bit. */
    backup_prcr = (uint16_t)(SYSTEM.PRCR.WORD | 0xA500);

    /* ---- Enable write protection ---- */
    /* PRCR - Protect Register 
    b15:b8   PRKEY    - PRC Key Code - A5h 
                        (The write value should be A5h to permission writing PRCi bit)
    b7:b4    Reserved - The write value should be 0.
    b3       PRC3     - Protect Bit 3 - Write disabled
    b2       Reserved - The write value should be 0.
    b1       PRC1     - Protect Bit 1 - Write enabled
    b0       PRC0     - Protect Bit 0 - Write disabled */
    SYSTEM.PRCR.WORD = 0xA502;

    cmt_int_cnt = 0;

    /* ---- Exit the module stop state ---- */
    MSTP(CMT0) = 0;                         /* CMT0 and CMT1 exit the module stop state. */

    /* ---- Stop CMT0 counting ---- */
    /* CMSTR0 - Compare Match Timer Start Register 0
    b15:b2   Reserved - The write value should be 0.
    b1       STR1     - Count Start 1 - CMT1.CMCNT count is stopped.
    b0       STR0     - Count Start 0 - CMT0.CMCNT count is stopped. */
    CMT.CMSTR0.BIT.STR0 = 0;                /* CMT0 count is stopped. */

    /* ---- Set the CMT0 count source and Enable the CMT0 compare match interrupt ---- */
    /* CMCR - Compare Match Timer Control Register
    b15:b8   Reserved - The write value should be 0.
    b7       Reserved - The write value should be 1.
    b6       CMIE     - Compare Match Interrupt Enable - Compare match interrupt enable.
    b5:b2    Reserved - The write value should be 0.
    b1:b0    CKS      - Clock Select - PCLK/32 */
    CMT0.CMCR.WORD = 0x00C1;

    /* ---- Clear the CMT0 count ---- */
    /* CMCNT - Compare Match Timer Counter */
    CMT0.CMCNT = 0x0000;

    /* ---- Wait time set is other than 0? ---- */
    if (0 != cnt) 
    {
        /* ---- Decrement the wait time  & wait counter ---- */
        cnt         = cnt - 1;
        cmt_int_cnt = ((cnt & 0xFFFF0000) >> 16) + 1;
        cnt         = cnt & 0x0000FFFF;

    }

    /* ---- Set the CMT0 wait time ---- */
    /* CMCOR - Compare Match Timer Constant Register */
    /* Because the access size of the CMCOR is 16bit,
       the cnt variable is changed from a uint32_t type to a uint16_t type. */
    CMT0.CMCOR = (uint16_t)cnt;

    /* ---- Clear the CMT0 interrupt request ---- */
    while (1 == IR(CMT0,CMI0)) 
    {
        IR(CMT0,CMI0) = 0;                  /* CMT0.CMI0 interrupt request is cleared. */
    }

    /* ---- Start CMT0 counting ---- */
    /* CMSTR0 - Compare Match Timer Start Register 0
    b0       STR0     - Count Start 0 - CMT0.CMCNT count is started. */
    CMT.CMSTR0.BIT.STR0 = 1;

    for ( ; cmt_int_cnt > 0; cmt_int_cnt--) 
    {

        /* ---- CMI0 interrupt request generated? ---- */
        while (0 == IR(CMT0,CMI0)) 
        {
        	/* Do nothing */
        }

        /* Clear IR flag */
        IR(CMT0,CMI0) = 0;

        /* ---- Set the CMT0 count source and Enable the CMT0 compare match interrupt ---- */
        /* CMCR - Compare Match Timer Control Register
        b6       CMIE     - Compare Match Interrupt Enable - Compare match interrupt enable.
        b1:b0    CKS      - Clock Select - PCLK/32 */
        CMT0.CMCR.WORD = 0x00C1;

        /* CMCOR - Compare Match Timer Constant Register */
        CMT0.CMCOR = 0xFFFF;

    }

    /* ---- Stop CMT0 counting ---- */
    /* CMSTR0 - Compare Match Timer Start Register 0
    b0      STR0      - Count Start 0 - CMT0.CMCNT count is stopped. */
    CMT.CMSTR0.BIT.STR0 = 0;

    /* ---- Initialize CMT0 ---- */
    /* CMCR - Compare Match Timer Control Register
    b6       CMIE     - Compare Match Interrupt Enable - Compare match interrupt disable.
    b1:b0    CKS      - Clock Select - PCLK/8 */
    CMT0.CMCR.WORD = 0x0080;

    /* CMCNT - Compare Match Timer Counter */
    CMT0.CMCNT = 0x0000;

    /* CMCOR - Compare Match Timer Constant Register */
    CMT0.CMCOR = 0x0000;

    /* Clear IR flag */
    IR(CMT0,CMI0) = 0;

    /* ----Transfer the module stop state ---- */
    MSTP(CMT0) = 1;

    SYSTEM.MSTPCRA.LONG = backup_mstp;

    /* ---- Disable write protection ---- */
    /* PRCR - Protect Register 
    b15:b8   PRKEY    - PRC Key Code - A5h 
                        (The write value should be A5h to permission writing PRCi bit)
    b1       PRC1     - Protect Bit 1 - Write disabled
    b0       PRC0     - Protect Bit 0 - Write disabled */
    SYSTEM.PRCR.WORD = backup_prcr;

}

/* End of File */

