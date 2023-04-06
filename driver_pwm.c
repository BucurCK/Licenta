#include "driver_pwm.h"
#include "main.h"

int16_t pwm_value = 3600;			//PWM max Value
int16_t sat_pwm_max = 30000;		//Saturation Value for PWM
int16_t sat_pwm_min = -30000;		//Saturation Value for PWM

void pwm_init (void)
{
///*
// * PWM pins configuration CCU80 - as alter function 3 - CCU80 OUTxy
// */
//	PORT5->IOCR8 |= (GPIO_OUTPUT_ALTER_3 << PORT5_PC11_POS); 		// A_HIGH 	- P5.11 - OUT00
//	PORT5->IOCR8 |= (GPIO_OUTPUT_ALTER_3 << PORT5_PC8_POS);			// A_LOW 	- P5.8  - OUT01
//
//	PORT0->IOCR4 |= (GPIO_OUTPUT_ALTER_3 << PORT0_IOCR4_PC4_Pos);	// B_HIGH 	- P5.10 - OUT10 P0.4 alternative
//	PORT2->IOCR12 |=(GPIO_OUTPUT_ALTER_3 << PORT2_PC15_POS);		// B_LOW 	- P2.15 - OUT11
//
//	PORT5->IOCR8 |= (GPIO_OUTPUT_ALTER_3 << PORT5_PC9_POS);			// C_HIGH 	- P5.9	- OUT20
//	PORT2->IOCR12 |=(GPIO_OUTPUT_ALTER_3 << PORT2_PC14_POS);		// C_LOW 	- P2.14	- OUT21

/*
 * CCU80 is already set up from VADC_irq
 */
//	/*  Reset CCU80*/
//	SCU_RESET->PRSET0 |= SCU_PRSET0_CCU80RS_MSK;
//
//	/* Clear reset of CCU80 */
//	SCU_RESET->PRCLR0 |= SCU_PRCLR0_CCU80RS_MSK;
//
//	/* Enable CCU8 via clkset */
//	SCU_CLK->CLKSET |= SCU_CLKSET_CCUCEN_MSK;
//
//	/* Enable prescaler */
//	CCU80->GIDLC |= CCU8_GIDLC_SPRB_MSK;

/*
 * Counting Mode - center aligned(0->max->0)
 */
	CCU80_CC80->TC |= (uint32_t)(CCU8_CC8_TC_TCM_MSK);
	CCU80_CC81->TC |= (uint32_t)(CCU8_CC8_TC_TCM_MSK);
	CCU80_CC82->TC |= (uint32_t)(CCU8_CC8_TC_TCM_MSK);

/*
 * Period&Compare channel 1 values configuration
 */
	CCU80_CC80->PRS |= (uint32_t)(CCU8_CC8_PRS_PR_VALUE);   //Shadow value to timer period PR
	CCU80_CC80->CR1S |=(uint32_t)(CCU8_CC8_CR1_CR1_VALUE);	//Shadow value to compare channel 1

	CCU80_CC81->PRS |= (uint32_t)(CCU8_CC8_PRS_PR_VALUE);
	CCU80_CC81->CR1S |=(uint32_t)(CCU8_CC8_CR1_CR1_VALUE);

	CCU80_CC82->PRS |= (uint32_t)(CCU8_CC8_PRS_PR_VALUE);
	CCU80_CC82->CR1S |=(uint32_t)(CCU8_CC8_CR1_CR1_VALUE);


/*
 *  Shadow transfer enable for CCU80_80/81/82    Request shadow transfer
 */
	CCU80->GCSS |= (CCU8_GCSS_S0SE_MSK | CCU8_GCSS_S1SE_MSK | CCU8_GCSS_S2SE_MSK);

/*
 * Configure timers
 */
/*
 * Remove from IDLE 80-82
 */
	CCU80->GIDLC |= (CCU8_GIDLC_CS0I_MSK | CCU8_GIDLC_CS1I_MSK | CCU8_GIDLC_CS2I_MSK) ;

/*   Clear the timer run bit(TRBC) and timer (TCC) */
	CCU80_CC80->TCCLR |= (uint32_t)(CCU8_CC8_TCCLR_TRBC_MSK);
	CCU80_CC80->TCCLR |= (uint32_t)(CCU8_CC8_TCCLR_TCC_MSK);

	CCU80_CC81->TCCLR |= (uint32_t)(CCU8_CC8_TCCLR_TRBC_MSK);
	CCU80_CC81->TCCLR |= (uint32_t)(CCU8_CC8_TCCLR_TCC_MSK);

	CCU80_CC82->TCCLR |= (uint32_t)(CCU8_CC8_TCCLR_TRBC_MSK);
	CCU80_CC82->TCCLR |= (uint32_t)(CCU8_CC8_TCCLR_TCC_MSK);

/*
 * Deadband config - enable for channel 1 and ST1
 */
	CCU80_CC80->DTC |= CCU8_CC8_DTC_DTE1_MSK | CCU8_CC8_DTC_DCEN1_MSK;
	CCU80_CC81->DTC |= CCU8_CC8_DTC_DTE1_MSK | CCU8_CC8_DTC_DCEN1_MSK;
	CCU80_CC82->DTC |= CCU8_CC8_DTC_DTE1_MSK | CCU8_CC8_DTC_DCEN1_MSK;

/*
 * Deadband value rising config - rising 50ns
 */
	CCU80_CC80->DC1R |= CCU8_CC8_DC1R_Value << CCU8_CC8_DC1R_DT1R_POS;
	CCU80_CC81->DC1R |= CCU8_CC8_DC1R_Value << CCU8_CC8_DC1R_DT1R_POS;
	CCU80_CC82->DC1R |= CCU8_CC8_DC1R_Value << CCU8_CC8_DC1R_DT1R_POS;

/*
 * Input selector & matrix
 * Use IN_H for Event 0 on Rising edge
 * Set start on EVENT 0 on Rising edge
 * Set stop on Event 1 on Falling edge
 */
	CCU80_CC80->INS |= CCU8_IN_H << CCU8_CC8_INS_EV0IS_POS;		//Set Event 0(system control) as rising edge
	CCU80_CC80->INS |= RISING_EDGE << CCU8_CC8_INS_EV0EM_POS;
	CCU80_CC80->INS |= CCU8_IN_H << CCU8_CC8_INS_EV1IS_POS;		//Set Event 1(system control) as falling edge
	CCU80_CC80->INS |= FALLING_EDGE << CCU8_CC8_INS_EV1EM_POS;

	CCU80_CC81->INS |= CCU8_IN_H << CCU8_CC8_INS_EV0IS_POS;
	CCU80_CC81->INS |= RISING_EDGE << CCU8_CC8_INS_EV0EM_POS;
	CCU80_CC81->INS |= CCU8_IN_H << CCU8_CC8_INS_EV1IS_POS;
	CCU80_CC81->INS |= FALLING_EDGE << CCU8_CC8_INS_EV1EM_POS;

	CCU80_CC82->INS |= CCU8_IN_H << CCU8_CC8_INS_EV0IS_POS;
	CCU80_CC82->INS |= RISING_EDGE << CCU8_CC8_INS_EV0EM_POS;
	CCU80_CC82->INS |= CCU8_IN_H << CCU8_CC8_INS_EV1IS_POS;
	CCU80_CC82->INS |= FALLING_EDGE << CCU8_CC8_INS_EV1EM_POS;

	CCU80_CC80->CMC |= CCU8_CC8_CMC_STRTS_EV0_MSK << CCU8_CC8_CMC_STRTS_POS;	//Set Event 0 for start
	CCU80_CC81->CMC |= CCU8_CC8_CMC_STRTS_EV0_MSK << CCU8_CC8_CMC_STRTS_POS;
	CCU80_CC82->CMC |= CCU8_CC8_CMC_STRTS_EV0_MSK << CCU8_CC8_CMC_STRTS_POS;


	CCU80_CC80->CMC |= CCU8_CC8_CMC_ENDS_EV1_MSK << CCU8_CC8_CMC_ENDS_POS;		//Set Event 1 for end
	CCU80_CC81->CMC |= CCU8_CC8_CMC_ENDS_EV1_MSK << CCU8_CC8_CMC_ENDS_POS;
	CCU80_CC82->CMC |= CCU8_CC8_CMC_ENDS_EV1_MSK << CCU8_CC8_CMC_ENDS_POS;

/*
 * Enable CCUCON Module
 */
	SCU_GENERAL->CCUCON |= SCU_GENERAL_CCUCON_GSC80_MSK;
//	SCU_GENERAL->CCUCON &= ~SCU_GENERAL_CCUCON_GSC80_MSK; 		//Reset back to 0 if it needs to be re-enabled later
}
/*
 * Sets CCUCON to 1 which enables the 0->1 transition (Rising Edge)
 */
void pwm_timer_start (void)
{
	SCU_GENERAL->CCUCON |= SCU_GENERAL_CCUCON_GSC80_MSK;
}
/*
 * Sets CCUCON to 0 which enables the 1->0 transition (Falling Edge)
 */
void pwm_timer_stop (void)
{
	SCU_GENERAL->CCUCON &= ~SCU_GENERAL_CCUCON_GSC80_MSK;
}
/*
 * Functions that updates the compare values in the PWM
 * Input type: int16_t
 */
void pwm_update (int16_t uaref,int16_t ubref,int16_t ucref)
{
	/*
	 * Keeps the voltage values between a range of max/min saturation
	 * We don't want the resulted PWM to be 0 or MAX
	 */
	if (uaref > sat_pwm_max)
	{
		uaref = sat_pwm_max;
	}
	else if (uaref < sat_pwm_min)
	{
		uaref = sat_pwm_min;
	}

	if (ubref > sat_pwm_max)
	{
		ubref = sat_pwm_max;
	}
	else if (ubref < sat_pwm_min)
	{
		ubref = sat_pwm_min;
	}

	if (ucref > sat_pwm_max)
	{
		ucref = sat_pwm_max;
	}
	else if (ucref < sat_pwm_min)
	{
		ucref = sat_pwm_min;
	}

	//New compare value
	CCU80_CC80->CR1S = (pwm_value/2 - pwm_value * uaref/65535);
	CCU80_CC81->CR1S = (pwm_value/2 - pwm_value * ubref/65535);
	CCU80_CC82->CR1S = (pwm_value/2 - pwm_value * ucref/65535);

	//Shadow transfer enabled for new compare value
	CCU80->GCSS |= (CCU8_GCSS_S0SE_MSK | CCU8_GCSS_S1SE_MSK | CCU8_GCSS_S2SE_MSK);
}

void motion_on (void)
{
/*
 * PWM pins configuration CCU80 - as alter function 3 - CCU80 OUTxy
 */
	PORT5->IOCR8 |= (GPIO_OUTPUT_ALTER_3 << PORT5_PC11_POS); 		// A_HIGH 	- P5.11 - OUT00
	PORT5->IOCR8 |= (GPIO_OUTPUT_ALTER_3 << PORT5_PC8_POS);			// A_LOW 	- P5.8  - OUT01

	PORT0->IOCR4 |= (GPIO_OUTPUT_ALTER_3 << PORT0_IOCR4_PC4_Pos);	// B_HIGH 	- P5.10 - OUT10 P0.4 alternative
	PORT2->IOCR12 |=(GPIO_OUTPUT_ALTER_3 << PORT2_PC15_POS);		// B_LOW 	- P2.15 - OUT11

	PORT5->IOCR8 |= (GPIO_OUTPUT_ALTER_3 << PORT5_PC9_POS);			// C_HIGH 	- P5.9	- OUT20
	PORT2->IOCR12 |=(GPIO_OUTPUT_ALTER_3 << PORT2_PC14_POS);		// C_LOW 	- P2.14	- OUT21
}

void motion_off(void)
{
/*
 * PWM pins configuration CCU80 - OFF
 */
	PORT5->IOCR8 &= ~(DISABLE_OUTPUT << PORT5_PC11_POS); 		// A_HIGH 	- P5.11 - OUT00
	PORT5->IOCR8 &= ~(DISABLE_OUTPUT << PORT5_PC8_POS);			// A_LOW 	- P5.8  - OUT01

	PORT0->IOCR4 &= ~(DISABLE_OUTPUT << PORT0_IOCR4_PC4_Pos);	// B_HIGH 	- P5.10 - OUT10 P0.4 alternative
	PORT2->IOCR12 &=~(DISABLE_OUTPUT << PORT2_PC15_POS);		// B_LOW 	- P2.15 - OUT11

	PORT5->IOCR8 &= ~(DISABLE_OUTPUT << PORT5_PC9_POS);			// C_HIGH 	- P5.9	- OUT20
	PORT2->IOCR12 &=~(DISABLE_OUTPUT << PORT2_PC14_POS);		// C_LOW 	- P2.14	- OUT21

}




