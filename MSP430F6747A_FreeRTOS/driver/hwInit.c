#include "hwInit.h"
#include "uart_api.h"

/**********************************************************************************************/

void startTickTimer()
{
  TA0CTL = 0;

  /*Stoppe den Timer, Clocksource ist ACLK, Divisor ist 1.*/
  TA0CTL = TASSEL_1;

  /*Stelle den Comparewert ein*/
  TA0CCR0 = (configLFXT_CLOCK_HZ / configTICK_RATE_HZ);

  /*Aktiviere den Compare 1 Interrupt.*/
  TA0CCTL0 = CCIE;

  /*Fuehre Timer clear aus und starte den Timer.*/
  TA0CTL |= (TACLR | MC_1);
}


/**********************************************************************************************/

/*
 * Callback der von FreeRTOS zum Einstellen des Ticktimers aufgerufen wird.
 */
void vApplicationSetupTimerInterrupt(void)
{
  startTickTimer();
}

static void initializeGpio(void)
{
  //Configure the UART pins.
  P3DIR = BIT7; //Tx
  P3REN = BIT6; //Pull Up on Rx.
}


static void initializeClocksAndVoltage(void)
{

  //VCore=2.4V->max. MCLK=25MHz.
  PMM_setVCore(PMM_CORE_LEVEL_3);

  UCS_setExternalClockSource(XT1_CLK, XT2_CLK);

  //DCO FLL Reference = XT1.
  UCS_initClockSignal(UCS_FLLREF, UCS_XT1CLK_SELECT, UCS_CLOCK_DIVIDER_1);

  //ACLK = XT1.
  UCS_initClockSignal(UCS_ACLK, UCS_XT1CLK_SELECT, UCS_CLOCK_DIVIDER_1);

  //Initialize the DCO.
  UCS_initFLLSettle(UCS_MCLK_DESIRED_FREQUENCY_IN_KHZ,
      ((uint32_t) UCS_MCLK_DESIRED_FREQUENCY_IN_KHZ * (uint32_t) 1000) / (uint32_t) XT1_CLK);
}

void initializeMainComponents(void)
{

  initializeClocksAndVoltage();

  initializeGpio();

  uart_initializeDebug();

  _enable_interrupt();

}

