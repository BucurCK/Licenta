#include "timer.h"
#include "main.h"
#include "kernel_interrupt.h"
#include "driver_pwm.h"

/*
 * Initialize CC81_81 as delay timer -- NOT USED
 */
void init_timer(void)
{
	/*  Reset CCU81*/
	SCU_RESET->PRSET0 |= SCU_RESET_PRSET0_CCU81RS_Msk;

	/* Clear reset of CCU81 */
	SCU_RESET->PRCLR0 |= SCU_RESET_PRCLR0_CCU81RS_Msk;

	/* Enable CCU8 via clkset */
	SCU_CLK->CLKSET |= SCU_CLKSET_CCUCEN_MSK;

	/* Enable prescaler */
	CCU81->GIDLC |= CCU8_GIDLC_SPRB_Msk;

	/*Set timer to edge aligned mode */
	CCU81_CC80->TC &= ~(CCU8_CC8_TC_TCM_Msk);

	/*SHADOW:Define the period value of Compare Channel 1 */
	CCU81_CC80->PRS |= (CCU8_CC8_PRS_PR_MS_VALUE << CCU8_CC8_PR_PR_Pos);
	CCU81_CC80->CR1S |=(50U); // Shadow value to compare channel 1


	/*
	 *  Shadow transfer enable for CCU81_80
	 */
	CCU81->GCSS |= CCU8_GCSS_S0SE_MSK;

	/*
	 * Remove from IDLE 80-82
	 */
	CCU81->GIDLC |= CCU8_GIDLC_CS0I_MSK;

	/*   Clear the timer run bit(TRBC) and timer (TCC) */
	CCU81_CC80->TCCLR |= (uint32_t)(CCU8_CC8_TCCLR_TRBC_MSK);
	CCU81_CC80->TCCLR |= (uint32_t)(CCU8_CC8_TCCLR_TCC_MSK);

	CCU81_CC80->INS |= CCU8_IN_H << CCU8_CC8_INS_EV0IS_POS; // Set Event 0(system control) as rising edge
	CCU81_CC80->INS |= RISING_EDGE << CCU8_CC8_INS_EV0EM_POS;

	CCU81_CC80->CMC |= CCU8_CC8_CMC_STRTS_EV0_MSK << CCU8_CC8_CMC_STRTS_POS; // Set Event 0 for start

	//2638 - configure the timer CCU8 to generate  a service request when matching compare channel 1 while counting down
//	CCU81_CC80->INS  |=  15 << CCU8_CC8_INS_EV2IS_Pos;   //which input is used -> input CC80.IN3P
//	CCU81_CC80->INS  &=  ~CCU8_CC8_INS_EV2EM_Msk;	 // event active on rising edge
//	CCU81_CC80->INS  |=  1  << CCU8_CC8_INS_EV2EM_Pos;	 // event active on rising edge

	//2672 - Enables the Channel 1 compare match while counting up interrupt
	CCU81_CC80->INTE |= 1 << CCU8_CC8_INTE_CMU1E_Pos;

	//2675 - The interrupt generated by compare match of channel 1 is forwarded to CC8ySR2
//	CCU81_CC80->SRS  |= 2 << CCU8_CC8_SRS_CM1SR_Pos;

	NVIC_ClearPendingIRQ(CCU81_0_IRQn);
	NVIC_EnableIRQ(CCU81_0_IRQn); //enable irq

	//This function configures ccu80 timer  to priority level 30
	NVIC_SetPriority(CCU81_0_IRQn, 20U);
	//Channel 1 compare match service request selector

	/*
	 * Enable CCUCON Module
	 */
	SCU_GENERAL->CCUCON |= SCU_GENERAL_CCUCON_GSC81_Msk;
	//	SCU_GENERAL->CCUCON &= ~SCU_GENERAL_CCUCON_GSC80_MSK; 		//Reset back to 0 if it needs to be re-enabled later

}

/*
 * Wait for time*50us
 */
void delay_t(uint32_t time)
{
	__disable_irq();
	interrupt_counter_timer = 0;
	__enable_irq();
	while (interrupt_counter_timer < time)
	{
		// NO_ACTION
	};
}

/*
*Wait for time*ms -- Uses CC81_80 as clock set at 0.2s  -- NOT USED
*/
void delay_ms(uint32_t time_ms)
{
	uint32_t time_start = interrupt_counter_timer;
	uint32_t time_stop = interrupt_counter_timer + (time_ms * 5);

	if (time_stop < interrupt_counter_timer)
	{
		__disable_irq();
		interrupt_counter_timer = 0;
		__enable_irq();
		time_start = interrupt_counter_timer;
		time_stop = interrupt_counter_timer + (time_ms * 5);
	}

	while (time_start < time_stop){
		time_start = interrupt_counter_timer;
	}

}
