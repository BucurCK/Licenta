/*
 * driver_pwm.c
 *
 *  Created on: Jul 23, 2019
 *      Author: student
 */
#include <stdint.h>
#include <stdbool.h>
#include <xmc_common.h>
#include "driver_pwm.h"
#include "main.h"
#include "motor_data_update.h"
#include "Encoder.h"
#include "math.h"
#include "stdlib.h"
#include "transform.h"
#include "io.h"

/*
 * Initialize CCU40 and POSIF0
 */
//int16_t timer_value_old = 0;

/*Interrupt function for Eru0 used with ENCZ component of encoder*/
void ERU0_0_IRQHandler(void)
{
//	set_output_blink_1();						//TOGGLE LED1
//	ERU0->EXICON[2] &= ~ERU0_EXICON0_FL_MSK;	//Reset Status Flag for ETLx
}

void encoder_init (void)
{
/*
 * Pin set up
 */
	/*Digital Pad activated*/
	PORT14->PDISC &= ~(PORTS_PDISC_5_MSK);
	PORT14->PDISC &= ~(PORTS_PDISC_6_MSK);
	PORT14->PDISC &= ~(PORTS_PDISC_7_MSK);

	/*POSIF Pins*/
	PORT14->IOCR4 |= (INPUT_PULL_UP_MSK << PORT14_IOCR4_PC5_POS);	//P14.5 ENCZ
	PORT14->IOCR4 |= (INPUT_PULL_UP_MSK << PORT14_IOCR4_PC6_POS);	//P14.6	ENCB
	PORT14->IOCR4 |= (INPUT_PULL_UP_MSK << PORT14_IOCR4_PC7_POS);	//P14.7	ENCA

/*
 * Configure CCU40
 */
	/* Reset CCU40*/
	SCU_RESET->PRSET0 |= (SCU_PRESET0_CCU40RS_MSK);

	/* Clear reset of CCU40 */
	SCU_RESET->PRCLR0 |= (SCU_PRCLR0_CCU40RS_MSK);

	/* Enable CCU4 via clkset */
	SCU_CLK->CLKSET |= SCU_CLKSET_CCUCEN_MSK;

	/* Enable prescaler */
	CCU40->GIDLC |= CCU4_GIDLC_SPRB_MSK;

	/*Set clock as Module Clock*/
	CCU40->GCTRL &= ~(CCU4_GCTRL_PCIS_MOD_CLK_MSK << CCU4_GCTRL_PCIS_POS);

	/*Set Counting Mode - Edge Aligned*/
	CCU40_CC40->TC &= ~(CCU4_TC_TCM_EDGE_MSK);

	/*Period Value*/
	CCU40_CC40->PRS |= (CCU4_CC4_PRS_VALUE);

	/*Shadow transfer for PR*/
	CCU40->GCSS |= (CCU4_GCSS_S0SE_MSK);

/*
 * Configure POSIF0
 */
	/* Reset POSIF0*/
	SCU_RESET->PRSET0 |= (SCU_PRESET0_POSIF0RS_MSK);

	/* Clear reset of POSIF0*/
	SCU_RESET->PRCLR0 |= (SCU_PRCLR0_POSIF0RS_MSK);

	/*Quadrature Decoder Mode enabled*/
	POSIF0->PCONF |= POSIF_PCONF_FSEL_QUAD_MSK;

	/*Phase/Hal selector as INxB for the used pins*/
	POSIF0->PCONF |= (POSIF_IN_B_MSK << POSIF_PCONF_INSEL0_POS);
	POSIF0->PCONF |= (POSIF_IN_B_MSK << POSIF_PCONF_INSEL1_POS);
	POSIF0->PCONF |= (POSIF_IN_B_MSK << POSIF_PCONF_INSEL2_POS);

	/*Low Pass filter configuration - 4 clock cycles*/
	POSIF0->PCONF |= (POSIF_LOW_PASS_4 << POSIF_PCONF_LPC_POS);

/*
 * POSIF to CC40
 */
	/*Input selector configuration*/
	CCU40_CC40->INS |= (CCU4_IN_E_MSK << CCU4_INS_EV0IS_POS);	//Event 0 for Out0 on rising edge
	CCU40_CC40->INS |= (RISING_EDGE_ENC << CCU4_INS_EV0EM_POS);
	CCU40_CC40->INS |= (CCU4_IN_F_MSK << CCU4_INS_EV1IS_POS);	//Event 1 for Out1 on rising edge
	CCU40_CC40->INS |= (RISING_EDGE_ENC << CCU4_INS_EV1EM_POS);

	/*Connection Matrix Control*/
	CCU40_CC40->CMC |= (CCU4_EVENT_0_MSK << CCU4_CMC_CNTS_POS);	//Event0 set up for Counting
	CCU40_CC40->CMC |= (CCU4_EVENT_1_MSK << CCU4_CMC_UDS_POS);	//Event1 set up for Direction
/*
 * Stop and Start POSIF/CC40
 */
	/*Stop and reset the state machine of the quadrature decoder and the current status of the Hall sensor - POSIF*/
	POSIF0->PRUNC |= POSIF_PRUNC_CRB_MSK;	//Stops POSIF
	POSIF0->PRUNC |= POSIF_PRUNC_CSM_MSK;	//Resets POSIF

	/*Exit Idle CC40*/
	CCU40->GIDLC |= (CCU4_GIDLC_CS0I_MSK);

	/*Stop and clear timer*/
	CCU40_CC40->TCCLR |= (uint32_t)(CCU4_CC4_TCCLR_TRBC_Msk);
	CCU40_CC40->TCCLR |= (uint32_t)(CCU4_CC4_TCCLR_TCC_Msk);

	/*Start CCU40*/
	CCU40_CC40->TCSET |= (CCU4_TCSET_TRBS_MSK);

	/*Start POSIF0*/
	POSIF0->PRUNS |= (POSIF_PRUNS_SRB_MSK);

	/*Saves the first value of the timer*/
//	timer_value_old = CCU40_CC40->TIMER;
	timer_value_old_fast = CCU40_CC40->TIMER;

}

void encoder_interrupt_init (void) {

	/*Set P0.13 as input pin for ENC Z*/
	PORT0->IOCR12 |= (INPUT_PULL_UP_MSK << PORT0_IOCR12_PC13_POS);	//activate pull-up input on P0.13

	/*Start/reset/clear is done by default for ERU0*/

	/*Enable ERU0 2A2*/
	ERU0->EXISEL |= (ERU0_EXICEL_EXS2A_2A2_MSK << ERU0_EXICEL_EXS2A_2A2_POS);

	/*Event input control for ERU0_2A2 we need to configure EXICON2*/
	ERU0->EXICON[2] |= ERU0_EXICON0_PE_MSK; 								//The trigger pulse generation is enabled
	ERU0->EXICON[2] |= ERU0_EXICON0_RE_MSK; 								//A rising edge is considered as edge even
	ERU0->EXICON[2] &= ~(ERU0_EXICON0_SS_A_MSK << ERU0_EXICON0_SS_POS);		//Input A without additional combination

	/*Event output control for OGU0 (default path for ERU0)*/
	ERU0->EXOCON[0] |= (ERU0_EXOCON0_GP_MSK << ERU0_EXOCON0_GP_POS);		//ERU_GOUTy is always enabled and ERU_IOUTy becomes activated with each activation of ERU_TOUTy

	/*Interrupt enable*/
	NVIC_SetPriority(ERU0_0_IRQn, 2U);
	NVIC_ClearPendingIRQ(ERU0_0_IRQn);
	NVIC_EnableIRQ(ERU0_0_IRQn);

}









