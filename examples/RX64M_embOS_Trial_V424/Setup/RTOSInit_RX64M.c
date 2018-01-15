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
 File    : RTOSInit_RX64M.c for RENESAS RX64M and RSKRX64M eval board
 for Renesas CCRX, IAR and GNU compiler
 Purpose : Initializes and handles the hardware for embOS as far
 as required by embOS
 Feel free to modify this file acc. to your target system.
 --------  END-OF-HEADER  ---------------------------------------------
 */

#include "RTOS.h"
#include "trace_uart.h"

/*********************************************************************
 *
 *       Configuration
 *
 **********************************************************************
 */

/*********************************************************************
 *
 *       Clock frequency settings
 */
#ifndef   OS_FSYS                   /* Main clock frequency         */
#define OS_FSYS 120000000u        /* Assume 120MHz                */
#endif

#ifndef   OS_PCLK_TIMER             /* Peripheral clock for timer   */
#define OS_PCLK_TIMER (OS_FSYS/2u)
#endif

#ifndef   OS_TICK_FREQ
#define OS_TICK_FREQ (1000u)
#endif

#ifndef   OS_INIT_PLL
#define OS_INIT_PLL (1)
#endif

#ifndef   OS_INIT_SDRAM
#define OS_INIT_SDRAM (1)
#endif

/*********************************************************************
 *
 *       Configuration of communication to embOSView
 */
#ifndef   OS_VIEW_ENABLE            // Global enable of communication
#define OS_VIEW_ENABLE    (1)     // Default: on
#endif

#define OS_VIEW_USE_UART 1
#ifndef   OS_VIEW_USE_UART          // If set, UART will be used
#define OS_VIEW_USE_UART  (0)     // Default: 0 => Uart is not used
#endif                              // if OS_VIEW_ENABLE is on

/*********************************************************************
 *
 *       UART settings for embOSView
 *       If you do not want (or can not due to hardware limitations)
 *       to dedicate a UART to embOSView, please define it to be -1
 *       Currently UART7 is supported and the standard
 *       setup enables UART7 per default
 */
#ifndef   OS_UART
#define OS_UART (7)
#endif

#ifndef   OS_BAUDRATE
#define OS_BAUDRATE (115200u)
#endif

#ifndef   OS_PCLK_UART
#define OS_PCLK_UART  (OS_FSYS/2u)
#endif

/*********************************************************************
 *
 *       Configuration of Option Bytes and ID Code
 *
 *       We set all bytes explicitly to 0xFF which is the default value
 *       IAR handles these bytes in IAR linker files
 */
#ifdef __RX
#pragma address _MDEreg=0x00120064            // MDE register (Single Chip Mode)
#ifdef __BIG
const unsigned long _MDEreg = 0xFFFFFFF8; // Big endian
#else
const unsigned long _MDEreg = 0xFFFFFFFF; // Little endian
#endif
#pragma address _OFS1=0x00120068              // Option function select register 1 (OFS1)
const unsigned long _OFS1 = 0xFFFFFFFF;
#pragma address _OFS2=0x0012006C              // Option function select register 2 (OFS2)
const unsigned long _OFS2 = 0xFFFFFFFF;
#endif
#ifdef __GNUC__
//
// Please ensure that the section ".options" is defined in linker at fixed addresses 0x00120064.
//
#ifdef __RX_BIG_ENDIAN__                                                            // MDE register (Single Chip Mode)
const unsigned long _MDEreg __attribute__ ((section (".options"))) = 0xFFFFFFF8; // Big endian
#else
const unsigned long _MDEreg __attribute__ ((section (".options"))) = 0xFFFFFFFF; // Little endian
#endif
const unsigned long _OFS1 __attribute__ ((section (".options"))) = 0xFFFFFFFF; // Option function select register 1 (OFS1)
const unsigned long _OFS2 __attribute__ ((section (".options"))) = 0xFFFFFFFF;// Option function select register 2 (OFS2)
#endif
/****** End of configurable options *********************************/

/*********************************************************************
 *
 *       JLINKRX and UART settings for embOSView
 *
 *       Automatically generated from configuration settings
 */
#define OS_USE_JLINKRX    (OS_VIEW_ENABLE && (OS_VIEW_USE_UART == 0))

#define OS_UART_USED      ((OS_VIEW_ENABLE && (OS_VIEW_USE_UART != 0)) && (OS_UART == 7))

#if  (OS_UART_USED || OS_USE_JLINKRX)
#define OS_COM_INIT() OS_COM_Init()
#else
#define OS_COM_INIT()
#endif

#if OS_USE_JLINKRX
#include "JLINKRX.h"
#endif

/*********************************************************************
 *
 *       Compiler dependent defines
 *
 **********************************************************************
 */
#ifdef __ICCRX__
#define EVENACCESS
#define WAIT_FOR_INTERRUPT()   __wait_for_interrupt()
#endif

#ifdef __RX
#define EVENACCESS             __evenaccess
#define WAIT_FOR_INTERRUPT()   wait()
#endif

#ifdef __GNUC__
#define EVENACCESS
#define WAIT_FOR_INTERRUPT()   __builtin_rx_wait()
#endif

/*********************************************************************
 *
 *   Sfrs used in RTOSInit
 *
 **********************************************************************
 */

/****** Protect register  ********************************************/
#define PRCR                 (*(volatile EVENACCESS OS_U16*) (0x000803FEu))

/****** Interrupt controller       **********************************/
#define IRR_BASE_ADDR        (0x00087000u)
#define ISEL_BASE_ADDR       (0x00087100u)

#define IER03                (*(volatile EVENACCESS OS_U8*)  (0x00087203u))
#define IER0C                (*(volatile EVENACCESS OS_U8*)  (0x0008720Cu))

#define IPR04                (*(volatile EVENACCESS OS_U8*)  (0x00087304u))
#define IPR98                (*(volatile EVENACCESS OS_U8*)  (0x00087362u))
#define IPR99                (*(volatile EVENACCESS OS_U8*)  (0x00087363u))

/****** SYSTEM module  **********************************************/
#define SYSTEM_BASE_ADDR     (0x00080000u)
#define SYSCR0               (*(volatile EVENACCESS OS_U16*) (SYSTEM_BASE_ADDR + 0x006u))
#define MSTPCRA              (*(volatile EVENACCESS OS_U32*) (SYSTEM_BASE_ADDR + 0x010u))
#define MSTPCRB              (*(volatile EVENACCESS OS_U32*) (SYSTEM_BASE_ADDR + 0x014u))
#define MSTPCRC              (*(volatile EVENACCESS OS_U32*) (SYSTEM_BASE_ADDR + 0x018u))
#define MSTPCRD              (*(volatile EVENACCESS OS_U32*) (SYSTEM_BASE_ADDR + 0x01Cu))

#define SCKCR                (*(volatile EVENACCESS OS_U32*) (SYSTEM_BASE_ADDR + 0x020u))
#define SCKCR2               (*(volatile EVENACCESS OS_U16*) (SYSTEM_BASE_ADDR + 0x024u))
#define SCKCR3               (*(volatile EVENACCESS OS_U16*) (SYSTEM_BASE_ADDR + 0x026u))
#define PLLCR                (*(volatile EVENACCESS OS_U16*) (SYSTEM_BASE_ADDR + 0x028u))
#define PLLCR2               (*(volatile EVENACCESS OS_U8*)  (SYSTEM_BASE_ADDR + 0x02Au))
#define MOSCCR               (*(volatile EVENACCESS OS_U8*)  (SYSTEM_BASE_ADDR + 0x032u))
#define SOSCCR               (*(volatile EVENACCESS OS_U8*)  (SYSTEM_BASE_ADDR + 0x033u))
#define MOSCWTCR             (*(volatile EVENACCESS OS_U8*)  (SYSTEM_BASE_ADDR + 0x0A2u))

/****** MPC used for SDRAM init *************************************/
#define MPC_BASE_ADDR        (0x0008C100u)
#define PFAOE0               (*(volatile EVENACCESS OS_U8*) (MPC_BASE_ADDR + 0x04u))
#define PFAOE1               (*(volatile EVENACCESS OS_U8*) (MPC_BASE_ADDR + 0x05u))
#define PFBCR0               (*(volatile EVENACCESS OS_U8*) (MPC_BASE_ADDR + 0x06u))
#define PFBCR1               (*(volatile EVENACCESS OS_U8*) (MPC_BASE_ADDR + 0x07u))

/****** BSC for SDRAM init *************************************/
#define BSC_BASE_ADDR        (0x00081300u)
#define SDAMOD               (*(volatile EVENACCESS OS_U8*)  (BSC_BASE_ADDR + 0x02902u))
#define SDIR                 (*(volatile EVENACCESS OS_U16*) (BSC_BASE_ADDR + 0x02924u))
#define SDICR                (*(volatile EVENACCESS OS_U8*)  (BSC_BASE_ADDR + 0x02920u))
#define SDSR                 (*(volatile EVENACCESS OS_U8*)  (BSC_BASE_ADDR + 0x02950u))
#define SDCMOD               (*(volatile EVENACCESS OS_U8*)  (BSC_BASE_ADDR + 0x02901u))
#define SDCCR                (*(volatile EVENACCESS OS_U8*)  (BSC_BASE_ADDR + 0x02900u))
#define SDMOD                (*(volatile EVENACCESS OS_U16*) (BSC_BASE_ADDR + 0x02948u))
#define SDRFCR               (*(volatile EVENACCESS OS_U16*) (BSC_BASE_ADDR + 0x02914u))
#define SDTR                 (*(volatile EVENACCESS OS_U32*) (BSC_BASE_ADDR + 0x02944u))
#define SDADR                (*(volatile EVENACCESS OS_U8*)  (BSC_BASE_ADDR + 0x02940u))
#define SDRFEN               (*(volatile EVENACCESS OS_U8*)  (BSC_BASE_ADDR + 0x02916u))

#define PORTA_DSCR           (*(volatile EVENACCESS OS_U8*)  (0x00008C0EAu))
#define PORTB_DSCR           (*(volatile EVENACCESS OS_U8*)  (0x00008C0EBu))
#define PORTD_DSCR           (*(volatile EVENACCESS OS_U8*)  (0x00008C0EDu))
#define PORTE_DSCR           (*(volatile EVENACCESS OS_U8*)  (0x00008C0EEu))

/****** Assign timer used for OS ************************************/
#define CMT0_BASE_ADDR       (0x00088000u)
#define CMT0_MSTPCR          (MSTPCRA)
#define CMT0_MSTPCR_BIT      (15u)
#define CMT0_VECT            28u
#define CMT0_IPR             (IPR04)
#define CMT0_IER             (IER03)
#define CMT0_IER_BIT         (4u)
#define OS_TIMER_INT_PRIO    (0x01u)     // Lowest priority
#define OS_TIMER_VECT        CMT0_VECT
#define TIMER_PRESCALE       (8u)
#define CMT0_CMSTR0          (*(volatile EVENACCESS OS_U16*) (CMT0_BASE_ADDR + 0x00u))
#define CMT0_CMCR            (*(volatile EVENACCESS OS_U16*) (CMT0_BASE_ADDR + 0x02u))
#define CMT0_CMCNT           (*(volatile EVENACCESS OS_U16*) (CMT0_BASE_ADDR + 0x04u))
#define CMT0_CMCOR           (*(volatile EVENACCESS OS_U16*) (CMT0_BASE_ADDR + 0x06u))

/****** UART ********************************************************/
#if OS_UART_USED

#define SCI7_BASE_ADDR       (0x0008A0E0u)

#define PWPR                 (*(volatile EVENACCESS OS_U8*)(0x0008C11Fu))
#define P90PFS               (*(volatile EVENACCESS OS_U8*)(0x0008C188u))
#define P92PFS               (*(volatile EVENACCESS OS_U8*)(0x0008C18Au))
#define P9PMR                (*(volatile EVENACCESS OS_U8*)(0x0008C069u))

#if (OS_UART == 7)
#define OS_UART_MSTPCR     (MSTPCRB)
#define OS_UART_MSTPCR_BIT (24u)
#define OS_UART_BASE_ADDR  (SCI7_BASE_ADDR)
#define OS_UART_IPR_RX     (IPR98)
#define OS_UART_IPR_TX     (IPR99)
#define OS_UART_IER_RX     (IER0C)
#define OS_UART_IER_TX     (IER0C)
#define OS_UART_IER_RX_BIT (2u)
#define OS_UART_IER_TX_BIT (3u)
#define OS_UART_RX_VECT     98u
#define OS_UART_TX_VECT     99u
#else
#error "Selected UART not supported, OS_UART_* definitions have to be modified."
#endif

#define OS_UART_INT_PRIO     (0x01u)     // Lowest priority

#define OS_UART_SMR          (*(volatile EVENACCESS OS_U8*) (OS_UART_BASE_ADDR + 0x00u))
#define OS_UART_BRR          (*(volatile EVENACCESS OS_U8*) (OS_UART_BASE_ADDR + 0x01u))
#define OS_UART_SCR          (*(volatile EVENACCESS OS_U8*) (OS_UART_BASE_ADDR + 0x02u))
#define OS_UART_TDR          (*(volatile EVENACCESS OS_U8*) (OS_UART_BASE_ADDR + 0x03u))
#define OS_UART_SSR          (*(volatile EVENACCESS OS_U8*) (OS_UART_BASE_ADDR + 0x04u))
#define OS_UART_RDR          (*(volatile EVENACCESS OS_U8*) (OS_UART_BASE_ADDR + 0x05u))
#define OS_UART_SCMR         (*(volatile EVENACCESS OS_U8*) (OS_UART_BASE_ADDR + 0x06u))

#define UART_ERR_PER_BIT     (3u)
#define UART_ERR_FER_BIT     (4u)
#define UART_ERR_ORER_BIT    (5u)
#define UART_ERR_PER_MASK    (1u << UART_ERR_PER_BIT)
#define UART_ERR_FER_MASK    (1u << UART_ERR_FER_BIT)
#define UART_ERR_ORER_MASK   (1u << UART_ERR_ORER_BIT)
#define UART_ERR_MASK_ALL    (UART_ERR_PER_MASK | UART_ERR_FER_MASK | UART_ERR_ORER_MASK)
#endif  // OS_UART_USED

/*********************************************************************
 *
 *       Local functions
 *
 **********************************************************************
 */

/*********************************************************************
 *
 *       _OS_GetHWTimerCycles()
 *
 * Function description
 *   Returns the current hardware timer count value
 *
 * Return value
 *   Current timer count value
 */
static unsigned int _OS_GetHWTimerCycles(void)
{
  return CMT0_CMCNT;
}

/*********************************************************************
 *
 *       _OS_GetHWTimer_IntPending()
 *
 * Function description
 *   Returns if the hardware timer interrupt pending flag is set
 *
 * Return value
 *   == 0; Interrupt pending flag not set
 *   != 0: Interrupt pending flag set
 */
static unsigned int _OS_GetHWTimer_IntPending(void)
{
  return *(OS_U8*) (IRR_BASE_ADDR + OS_TIMER_VECT);
}

/*********************************************************************
 *
 *       OS_InitHW()
 *
 *       Initialize the hardware (timer) required for embOS to run.
 *       May be modified, if an other timer should be used
 */
void OS_InitHW(void)
{
  //
  // Structure with information about timer frequency, tick frequency, etc.
  // for micro second precise system time.
  //
  OS_SYSTIMER_CONFIG SysTimerConfig = { OS_FSYS, OS_TICK_FREQ, 1, _OS_GetHWTimerCycles, _OS_GetHWTimer_IntPending };
  OS_IncDI();
  //
  // Initialize timer for embOS, assuming PLL is already initialized
  //
  PRCR = 0xA50Bu; // Protect off
  CMT0_MSTPCR &= ~(1u << CMT0_MSTPCR_BIT); // Activate CMT unit
  CMT0_CMSTR0 = 0x00u; // Stop counter
  CMT0_CMCR = (1u << 6u); // Compare match interrupt enable, PCLK/8
  CMT0_CMCOR = (OS_U16) ((OS_PCLK_TIMER / OS_TICK_FREQ) / TIMER_PRESCALE); // Count value for 1 msec resolution
  //
  // Enable CMT0 interrupt in interrupt controller
  //
  CMT0_IPR = OS_TIMER_INT_PRIO; // Set interrupt priority, lowest priority
  CMT0_IER |= (1u << CMT0_IER_BIT); // Enable CMT interrupt
  *(OS_U8*) (IRR_BASE_ADDR + OS_TIMER_VECT) = 0u; // Clear interrupt request
  *(OS_U8*) (ISEL_BASE_ADDR + OS_TIMER_VECT) = 0u; // Select interrupt source CPU
  OS_Config_SysTimer(&SysTimerConfig);
  //
  // Start timer
  //
  CMT0_CMSTR0 = 0x01u; // Start counter
  //
  // Initialize the optional UART for OS viewer
  //
  OS_COM_INIT();
  PRCR = 0xA500u; // Protect on
  OS_DecRI();
}

/*********************************************************************
 *
 *       OS_Idle()
 *
 *       Please note:
 *       This is basically the "core" of the idle loop.
 *       This core loop can be changed, but:
 *       The idle loop does not have a stack of its own, therefore no
 *       functionality should be implemented that relies on the stack
 *       to be preserved. However, a simple program loop can be programmed
 *       (like toggling an output or incrementing a counter)
 */
void OS_Idle(void)
{ // Idle loop: No task is ready to execute
  for (;;)
  { // Nothing to do ... wait for interrupt
#if ((OS_USE_JLINKRX == 0) && (OS_DEBUG == 0))     // Enter CPU halt mode when not in Debug build and J-Link communication not used
  WAIT_FOR_INTERRUPT(); // Switch CPU into sleep mode
#endif
  }
}

/*********************************************************************
 *
 *       OS_GetTime_Cycles()
 *
 *       This routine is required for task-info via embOSView or high
 *       resolution time measurement functions.
 *       It returns the system time in timer clock cycles.
 */
OS_U32 OS_GetTime_Cycles(void)
{
  OS_U32 Time;
  OS_U32 Cnt;

  Time = (OS_U32) OS_GetTime32();
  Cnt = CMT0_CMCNT;
  //
  // Check if timer interrupt pending ...
  //
  if ((*(volatile EVENACCESS OS_U8*) (IRR_BASE_ADDR + OS_TIMER_VECT) & (1u << 0u)) != 0u)
  {
    Cnt = CMT0_CMCNT; // Interrupt pending, re-read timer and adjust result
    Time++;
  }
  return (((OS_PCLK_TIMER / TIMER_PRESCALE) / OS_TICK_FREQ) * Time) + Cnt;
}

/*********************************************************************
 *
 *       OS_ConvertCycles2us()
 *
 *       Convert Cycles into micro seconds.
 *
 *       If your clock frequency is not a multiple of 1 MHz,
 *       you may have to modify this routine in order to get proper
 *       diagnostics.
 *
 *       This routine is required for profiling or high resolution time
 *       measurement only. It does not affect operation of the OS.
 */
OS_U32 OS_ConvertCycles2us(OS_U32 Cycles)
{
  Cycles *= TIMER_PRESCALE; // Eliminate effect of fixed prescaler
  return Cycles / (OS_PCLK_TIMER / 1000000u);
}

/*********************************************************************
 *
 *       OS_ISR_Tick()
 *       The OS system timer interrupt
 */

#pragma interrupt (OS_ISR_Tick(vect=OS_TIMER_VECT))
static void OS_ISR_Tick(void)
{
  OS_EnterNestableInterrupt();
  OS_TICK_Handle();
#if OS_USE_JLINKRX
  JLINKRX_Process();
#endif
  OS_LeaveNestableInterrupt()
  ;
}

/*********************************************************************
 *
 *   Communication for embOSView
 *
 **********************************************************************
 */
#if OS_USE_JLINKRX                    // Communication via J-Link RX

static void _JLINKRX_OnRx(OS_U8 Data);
static void _JLINKRX_OnTx(void);
static OS_INT _JLINKRX_GetNextChar(void);

/*********************************************************************
 *
 *       _JLINKRX_OnRx()
 */
static void _JLINKRX_OnRx(OS_U8 Data)
{
  OS_OnRx(Data);
}

/*********************************************************************
 *
 *       _JLINKRX_OnTx()
 */
static void _JLINKRX_OnTx(void)
{
  OS_OnTx();
}

/*********************************************************************
 *
 *       _JLINKRX_GetNextChar()
 */
static OS_INT _JLINKRX_GetNextChar(void)
{
  return OS_COM_GetNextChar();
}

/*********************************************************************
 *
 *       OS_COM_Init()
 *       Initialize memory access for embOSView
 */
void OS_COM_Init(void)
{
  JLINKRX_SetpfOnRx(_JLINKRX_OnRx);
  JLINKRX_SetpfOnTx(_JLINKRX_OnTx);
  JLINKRX_SetpfGetNextChar(_JLINKRX_GetNextChar);
}

/*********************************************************************
 *
 *       OS_COM_Send1()
 *       Send one character via memory
 */
void OS_COM_Send1(OS_U8 c)
{
  JLINKRX_SendChar(c);
}

#elif OS_UART_USED
#define OS_BAUDDIVIDE (((OS_PCLK_UART + (OS_BAUDRATE * 16u)) / (OS_BAUDRATE * 32u)) - 1u)



/*********************************************************************
 *
 *       OS_COM_Init()
 *       Initialize UART 1
 */
void OS_COM_Init(void)
{

  trace_initializeUart();

  volatile unsigned int i;
  //
  // Enable UART Module
  //
  OS_UART_MSTPCR &= ~(1u << OS_UART_MSTPCR_BIT);
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
  OS_UART_SCR = 0x00u;
  //
  // Set transmission mode
  //
  OS_UART_SMR = 0x00u;
  //
  // Set transfer direction, LSB first, 8 bit data length
  //
  OS_UART_SCMR = 0x10u;
  //
  // Set baudrate
  //
  OS_UART_BRR = (OS_U8) (OS_BAUDDIVIDE);
  //
  // Wait for SCI Settling time (1 bit cycle)
  //
  for (i = 0u; i < 3000u; i++)
  {
  }
  //
  // Clear status
  //
  OS_UART_SSR = 0x00u;
  //
  // Setup interrupt control register for UART
  //
  OS_UART_IPR_RX = OS_UART_INT_PRIO; // Lowest priority
  OS_UART_IPR_TX = OS_UART_INT_PRIO; // Lowest priority
  //
  // Enable UART interrupts
  //
  OS_UART_IER_RX |= (1u << OS_UART_IER_RX_BIT); // Enable Rx interrupt
  OS_UART_IER_TX |= (1u << OS_UART_IER_TX_BIT); // Enable Tx empty interrupt
  //
  // Clear interrupt request flags
  //
  *(OS_U8*) (IRR_BASE_ADDR + OS_UART_RX_VECT) = 0u;
  *(OS_U8*) (IRR_BASE_ADDR + OS_UART_TX_VECT) = 0u;
  //
  // Finally enable Rx and Tx interrupts and enable uart Rx and Tx
  //
  OS_UART_SCR |= (1u << 7u) // Tx interrupt enable
  | (1u << 6u); // Rx interrupt enable
  OS_UART_SCR |= (1u << 5u) // Tx enable
  | (1u << 4u); // Rx enable
}

/*********************************************************************
 *
 *       OS_COM_Send1()
 *       Send one character via UART.
 *       Never call this function from your application
 */
void OS_COM_Send1(OS_U8 c)
{
  OS_UART_TDR = c;
}

/*********************************************************************
 *
 *       _OS_ISR_Rx()
 *       Interrupt handler for UART reception
 */
#pragma interrupt (_OS_ISR_Rx(vect=OS_UART_RX_VECT)) // Rx data interrupt
static void _OS_ISR_Rx(void)
{
  OS_EnterNestableInterrupt();
#ifdef TRACE_WITH_UART
  trace_rxInterrupt(OS_UART_RDR);
#else
  OS_OnRx(OS_UART_RDR);
#endif
  OS_LeaveNestableInterrupt()
  ;
}

/*********************************************************************
 *
 *       _OS_ISR_Tx()
 *       Interrupt handler for UART transmission
 */
#pragma interrupt (_OS_ISR_Tx(vect=OS_UART_TX_VECT))  // TX data empty interrupt
static void _OS_ISR_Tx(void)
{
  OS_EnterNestableInterrupt();
#ifdef TRACE_WITH_UART
  trace_txInterrupt();
#else
  OS_OnTx();
#endif

  OS_LeaveNestableInterrupt();
}

#else  /* UART for communication not used, define dummy functions */

void OS_COM_Send1(OS_U8 c)
{
  OS_USEPARA(c); /* Avoid compiler warning */
  OS_COM_ClearTxActive(); /* Let the OS know that Tx is not busy */
}
#endif

/*********************************************************************
 *
 *       HardwareSetup()
 *
 *       Initialize clock generation and PLL
 *
 *       Has to be modified if another CPU clock frequency should be
 *       used. This function is called during startup and
 *       has to return 1 to perform segment initialization.
 *
 *       Because variables are not initialized before HardwareSetup() is called,
 *       no access or call which relies on any variable can be used here
 *       and no variables can be set to a specific value as long as those variables
 *       are not declared as "no init".
 *
 *       This function is called from startup code.
 */

void HardwareSetup(void)
{
//
// Initialize PLL, assuming external crystal of 24.0 MHz
//
//  Clock Description              Frequency
//  ----------------------------------------
//  Input Clock Frequency............  24 MHz
//  PLL frequency (x10).............. 240 MHz
//  Internal Clock Frequency......... 120 MHz
//  Peripheral Clock Frequency.......  60 MHz
//  USB Clock Frequency..............  48 MHz
//  External Bus Clock Frequency.....  60 MHz
//
#if OS_INIT_PLL
  volatile unsigned int i;
  PRCR = 0xA50Bu; // Protect off
  SOSCCR = 0x00u; // Enable sub-clock for RTC
  MOSCWTCR = 0x0Du; // Set wait time
  PLLCR = 0x1300u; // x10 @PLL
  MOSCCR = 0x00u; // EXTAL ON
  PLLCR2 = 0x00u; // PLL ON
  for (i = 0u; i < 0x168u; i++)
  { // Wait over 12ms
#ifdef __RX
    nop();
#endif
  }
//
// SCKCR - System Clock Control Register
//  FCK[3:0]    0x01 = Flash clock: PLL = (240 / 2) = 120 MHz
//  ICK[3:0]    0x01 = System clock: PLL = (240 / 2) = 120 MHz
//  PSTOP1      0x00 = BCLK pin output is enabled
//  BCK[3:0]    0x02 = BCLK: PLL/4 = 60 MHz
//  PCKA[3:0]   0x01 = Peripheral clock A: PLL/2 = 120 MHz
//  PCKB[3:0]   0x02 = Peripheral clock B: PLL/4 = 60 MHz
//  PCKC[3:0]   0x02 = Peripheral clock C: PLL/4 = 60 MHz
//  PCKD[3:0]   0x02 = Peripheral clock D: PLL/4 = 60 MHz
//
  SCKCR = 0x11021222u; // FCK,ICK=PLL/1,BCK,PCK=PLL/2
//
// SCKCR2 - System Clock Control Register 2
//  UCK[3:0]    0x03 = USB clock is PLL/5 = 48 MHz
//  IEBCK[3:0]  0x01 = IE Bus clock is PLL/2 = 96 MHz
//
  SCKCR2 = 0x0041u;
  SCKCR3 = 0x0400u; // ICLK, PCLKB, FCLK, BCLK, IECLK, and USBCLK all come from PLL circuit */

  PRCR = 0xA500u; // Protect on
#endif
//
// Set MSTPCRA, MSTPCRB and MSTPCRC to default setting
//
  MSTPCRA = 0x47FFFFFFu; // Set to initial value (Reset condition).
  MSTPCRB = 0xFFFFFFFFu; // All modules in MSTPCRB stop.
  MSTPCRC = 0xFFFF0000u; // b0: RAM0, 0 => enabled
                         // b1: RAM1, 0 => enabled
  MSTPCRD = 0xFFFFFF00u; // Set to initial value (Reset condition).

}

