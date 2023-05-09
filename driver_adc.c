#include <stdint.h>
#include <stdbool.h>
#include <xmc_common.h> // includes stdint stdbool stdef stdstring
#include "driver_pwm.h"
#include "main.h"
#include "motor_data_update.h"
#include "driver_adc.h"
#include "transform.h"
#include "io.h"
#include <probe_scope.h>
#include "Encoder.h"
#include "timer.h"
#include "motor_data_update.h"
#include "state_machine.h"
#include "pi_regulator.h"
#include "reference_generator.h"
#include "current_protection.h"

int32_t u_mot_dig, u_log_dig;			 // saves the 12bit value read from VADC
float_t u_mot, u_log;					 // The 12bit value converted into Volts
int32_t offset_ia, offset_ib, offset_ic; // Current offsets for every phase
int16_t ia, ib, ic;						 // Currents read from ADC in 16bit
float_t ia_a, ib_a, ic_a, iq_a;			 // Float values for Micrium
uint8_t prot_status = ON;				 // Status of Current protection -- OFF for offset computation

/*
	Initialize VADC unit
*/
void adc_init(void)
{
	/*Pin setup - P14.0 P14.3 P14.4*/
	PORT14->IOCR0 |= (INPUT_PULL_UP_MSK << PORT14_IOCR0_PC0_POS); // P14.0 - G0CH0				-A
	PORT14->IOCR0 |= (INPUT_PULL_UP_MSK << PORT14_IOCR0_PC3_POS); // P14.3 - G1CH3(ALIAS CH0)	-B
	PORT14->IOCR4 |= (INPUT_PULL_UP_MSK << PORT14_IOCR4_PC4_POS); // P14.4 - G2CH0				-C

	/*Disable Gating - enables the clock for VADC */
	SCU_CLK->CGATCLR0 |= SCU_CGATCLR0_VADC_MSK;

	/*Clear reset*/
	//	SCU_RESET->PRSET0 |= SCU_PRCLR0_VADCRS_MSK
	SCU_RESET->PRCLR0 |= SCU_PRCLR0_VADCRS_MSK;

	/*Enable Module Clock - write 0 to enable*/
	VADC->CLC &= ~VADC_CLC_DISR_MSK;

	/*Set Frequency to 36MHz by dividing the F_ADC/4 -> 144/4=36 --- NEED TO ALSO WRITE INTO ENABLE BIT*/
	//	VADC->GLOBCFG = (uint32_t)((VADC_GLOBCGF_DIVA_4_MSK << VADC_GLOBCGF_DIVA_POS) | (VADC_GLOBCFG_DIVWC));

	/*Configure Conversion resolution - 12BITS  --- GLOBAL*/
	VADC->GLOBICLASS[0] &= ~(VADC_GLOBICLASS0_CMS_12_MSK << VADC_GLOBICLASS0_CMS_POS);

	/*Configure Sample and hold time - 5clocks --- GLOBAL*/
	VADC->GLOBICLASS[0] |= (VADC_GLOBICLASS0_STCS_5_MSK << VADC_GLOBICLASS0_STCS_POS);

	/*Power Up VADC G0/G1/G2 for calibration*/
	/*Enable all Analog Converters*/
	VADC_G0->ARBCFG |= VADC_ARBCFG_ANONC_MSK;
	VADC_G1->ARBCFG |= VADC_ARBCFG_ANONC_MSK;
	VADC_G2->ARBCFG |= VADC_ARBCFG_ANONC_MSK;

	/*StartUp Calibration*/
	VADC->GLOBCFG |= ENABLE << VADC_GLOBCFG_SUCAL_POS;

	/*Wait for calibration to finish*/
	while ((VADC_G0->ARBCFG & VADC_ARBCFG_CAL_MSK) && (VADC_G1->ARBCFG & VADC_ARBCFG_CAL_MSK) && (VADC_G2->ARBCFG & VADC_ARBCFG_CAL_MSK))
	{
		// Wait for calibration
	};

	/*Configure the conversion kernel*/

	/*Slave SetUp - G1/G2*/
	/*Disable arbitration*/
	VADC_G1->ARBPR &= ~(DISABLE << VADC_ARBPR_ASEN0_POS);
	VADC_G2->ARBPR &= ~(DISABLE << VADC_ARBPR_ASEN0_POS);

	/*Set maximum priority */
	VADC_G1->ARBPR |= VADC_ARBPR_PRIO0_MAX_MSK;
	VADC_G2->ARBPR |= VADC_ARBPR_PRIO0_MAX_MSK;

	//	//GETS ALL THIS SETTINGS FROM MASTER - No need to set up slave settings
	//	/*Enable writing in TMEN - Timer mode for equidistant sampling enabled*/
	//	VADC_G1->QCTRL0 |= (ENABLE << VADC_QCTRL0_TMWC_POS) | (ENABLE << VADC_QCTRL0_TMEN_POS);
	//	VADC_G2->QCTRL0 |= (ENABLE << VADC_QCTRL0_TMWC_POS) | (ENABLE << VADC_QCTRL0_TMEN_POS);
	//
	//	/*Enable writing in XTMODE and XTSEL */
	//	/*Trigger Operating Mode - Trigger event upon any edge*/
	//	VADC_G1->QCTRL0 |= (ENABLE << VADC_QCTRL0_XTWC_POS) | (VADC_QCTRL0_XTMODE_ANY_MSK << VADC_QCTRL0_XTMODE_POS);
	//	VADC_G2->QCTRL0 |= (ENABLE << VADC_QCTRL0_XTWC_POS) | (VADC_QCTRL0_XTMODE_ANY_MSK << VADC_QCTRL0_XTMODE_POS);
	//
	//	/*External Trigger Input Selection - Trigger on Input J (CCU80.SR3)*/
	//	VADC_G1->QCTRL0 |= (ENABLE << VADC_QCTRL0_XTWC_POS) | (VADC_QCTRL0_XTSEL_J_MSK << VADC_QCTRL0_XTSEL_POS);
	//	VADC_G2->QCTRL0 |= (ENABLE << VADC_QCTRL0_XTWC_POS) | (VADC_QCTRL0_XTSEL_J_MSK << VADC_QCTRL0_XTSEL_POS);

	/*Enable interruption of other running conversions*/
	VADC_G1->ARBPR |= VADC_ARBPR_CSM0_MSK;
	VADC_G2->ARBPR |= VADC_ARBPR_CSM0_MSK;

	/*Enable arbitration*/
	VADC_G1->ARBPR |= (ENABLE << VADC_ARBPR_ASEN0_POS);
	VADC_G2->ARBPR |= (ENABLE << VADC_ARBPR_ASEN0_POS);

	/*Master SetUp - G0*/
	/*Disable arbitration*/
	VADC_G0->ARBPR &= ~(DISABLE << VADC_ARBPR_ASEN0_POS);

	/*Set maximum priority */
	VADC_G0->ARBPR |= VADC_ARBPR_PRIO0_MAX_MSK;

	/*Enable writing in TMEN - Timer mode for equidistant sampling enabled*/
	VADC_G0->QCTRL0 |= (ENABLE << VADC_QCTRL0_TMWC_POS) | (ENABLE << VADC_QCTRL0_TMEN_POS);

	/*Enable writing in XTMODE and XTSEL */
	/*Trigger Operating Mode - Trigger event upon any edge*/
	VADC_G0->QCTRL0 |= (ENABLE << VADC_QCTRL0_XTWC_POS) | (VADC_QCTRL0_XTMODE_ANY_MSK << VADC_QCTRL0_XTMODE_POS);

	/*External Trigger Input Selection - Trigger on Input J (CCU80.SR3)*/
	VADC_G0->QCTRL0 |= (ENABLE << VADC_QCTRL0_XTWC_POS) | (VADC_QCTRL0_XTSEL_J_MSK << VADC_QCTRL0_XTSEL_POS);

	/*Configure the Queue Mode*/
	/*Enable Gate - Conversion requests are issued if a valid conversion request is pending in the queue 0 register or in the backup register*/
	VADC_G0->QMR0 |= VADC_QMR0_ENGT_MSK;

	/*Enable External Trigger - The selected edge at the selected trigger input signal REQTR generates the trigger event*/
	VADC_G0->QMR0 |= VADC_QMR0_ENTR_MSK;

	/*Enable arbitration*/
	VADC_G0->ARBPR |= (ENABLE << VADC_ARBPR_ASEN0_POS);

	/*Channel configuration*/
	/*Set Channel 0 as priority channel*/
	VADC_G0->CHASS |= VADC_CHASS_ASSCH0_MSK;
	VADC_G1->CHASS |= VADC_CHASS_ASSCH0_MSK;
	VADC_G2->CHASS |= VADC_CHASS_ASSCH0_MSK;

	/*Convert G1CH3 into CH0 with ALIAS*/
	VADC_G1->ALIAS |= 3U << VADC_ALIAS_ALIAS0_POS;

	/*Channel Control - Request a synchronized conversion of this channel (only taken into account for a master)*/
	VADC_G0->CHCTR[0] |= VADC_CHCTR_SYNC_MSK;

	/*Use GLOBAL class0 configuration*/
	VADC_G0->CHCTR[0] |= VADC_CHCTR_ICSEL_GLOBAL_0_MSK;

	/*Register used to store results 0-15*/
	VADC_G0->CHCTR[0] |= (1U << VADC_G_CHCTR_RESREG_Pos);
	VADC_G1->CHCTR[0] |= (1U << VADC_G_CHCTR_RESREG_Pos);
	VADC_G2->CHCTR[0] |= (1U << VADC_G_CHCTR_RESREG_Pos);

	/*Configure the Result Input Queue for G0 Master				*/
	/*----Channel to be converted									*/
	/*Enable Source Interrupt - (x << VADC_G_QINR0_REQCHNR_Pos)		*/
	/*External Trigger - VADC_QINR0_EXTR_MSK						*/
	/*Automatic refill - VADC_QINR0_RF_MSK							*/
	/*End of conversion trigger - VADC_QINR0_ENSI_MSK				*/
	VADC_G0->QINR0 |= (0U << VADC_G_QINR0_REQCHNR_Pos) | VADC_QINR0_RF_MSK | VADC_QINR0_EXTR_MSK | VADC_QINR0_ENSI_MSK;
	;

	/*Configure master and slave channels*/
	/*Power down the VADC */
	VADC_G0->ARBCFG &= ~VADC_ARBCFG_ANONC_MSK;
	VADC_G1->ARBCFG &= ~VADC_ARBCFG_ANONC_MSK;
	VADC_G2->ARBCFG &= ~VADC_ARBCFG_ANONC_MSK;

	/*G0 – master; G1, G2 – slaves */
	VADC_G1->SYNCTR |= VADC_SYNCTR_STSEL_CL1_MSK;
	VADC_G2->SYNCTR |= VADC_SYNCTR_STSEL_CL1_MSK;

	/*Evaluate Ready Input Rx - EVALR1*/
	VADC_G0->SYNCTR |= VADC_SYNCTR_EVALR1_MSK;
	VADC_G1->SYNCTR |= VADC_SYNCTR_EVALR1_MSK;
	VADC_G2->SYNCTR |= VADC_SYNCTR_EVALR1_MSK;

	/*Power on the VADC*/
	VADC_G0->ARBCFG |= VADC_ARBCFG_ANONC_MSK;

	/*Result Register - Service Request Generation Enable*/
	VADC_G0->RCR[1] = VADC_GxRCRy_SRGEN_MSK;

	/*Routes the corresponding event trigger from RESULT group to G0_CH2*/
	VADC_G0->REVNP0 |= (VADC_REVNP0_REV0NP_L2G0_MSK << 4U);

	/*AutoScan for V_mot & V_log*/

	/*Pin Set up*/
	PORT15->IOCR4 |= (INPUT_PULL_UP_MSK << PORT15_IOCR4_PC6_Pos);	 // P15.6 - G2CH6				-Vmot
	PORT14->IOCR12 |= (INPUT_PULL_UP_MSK << PORT14_IOCR12_PC14_Pos); // P14.14- G1CH6				-Vlog

	/*Channel Select*/
	VADC_G1->ASSEL |= (ENABLE << VADC_ASSEL_CHSEL6_POS);
	VADC_G2->ASSEL |= (ENABLE << VADC_ASSEL_CHSEL6_POS);

	/*Define load event*/
	VADC_G1->ASMR |= VADC_ASMR_SCAN_MSK | VADC_ASMR_ENGT_MSK | VADC_ASMR_LDEV_MSK;
	VADC_G2->ASMR |= VADC_ASMR_SCAN_MSK | VADC_ASMR_ENGT_MSK | VADC_ASMR_LDEV_MSK;

	/*Conversion configuration to use global 0 settings*/
	VADC_G1->CHCTR[6] |= VADC_CHCTR_ICSEL_GLOBAL_0_MSK;
	VADC_G2->CHCTR[6] |= VADC_CHCTR_ICSEL_GLOBAL_0_MSK;

	/*Enable Arbitration*/
	VADC_G1->ARBPR |= (ENABLE << VADC_ARBPR_ASEN1_POS);
	VADC_G2->ARBPR |= (ENABLE << VADC_ARBPR_ASEN1_POS);

	/*Registers to store results 0-15*/
	VADC_G1->CHCTR[6] |= (14U << VADC_G_CHCTR_RESREG_Pos);
	VADC_G2->CHCTR[6] |= (14U << VADC_G_CHCTR_RESREG_Pos);
}

void interrupt_vadc_init(void)
{
	/*  Reset CCU80 */
	SCU_RESET->PRSET0 |= SCU_RESET_PRSET0_CCU80RS_Msk;

	/* Clear reset of CCU80 */
	SCU_RESET->PRCLR0 |= SCU_RESET_PRCLR0_CCU80RS_Msk;

	/* Enable CCU8 via clkset */
	SCU_CLK->CLKSET |= SCU_CLK_CLKSET_CCUCEN_Msk;

	/* Enable prescaler */
	CCU80->GIDLC |= CCU8_GIDLC_SPRB_Msk;

	/*Set timer to center aligned mode */
	CCU80_CC83->TC |= (uint32_t)(CCU8_CC8_TC_TCM_Msk);

	/*SHADOW:Define the period value of Compare Channel 1 */
	CCU80_CC83->PRS |= (uint32_t)(CCU8_CC8_PRS_PR_Value << CCU8_CC8_PR_PR_Pos);

	/*SHADOW:Define the compare value of Compare Channel 1 */
	CCU80_CC83->CR1S |= (uint32_t)(CCU80_CC83_CR_Value << CCU8_CC8_CR1_CR1_Pos);

	/*Request shadow transfer  */
	CCU80->GCSS |= (CCU8_GCSS_S3SE_Msk);

	/*Enable CC80*/
	CCU80->GIDLC |= (CCU8_GIDLC_CS3I_Msk);

	/*Clear the timer run bit(TRBC) and timer (TCC) */
	CCU80_CC83->TCCLR |= (uint32_t)(CCU8_CC8_TCCLR_TRBC_Msk);
	CCU80_CC83->TCCLR |= (uint32_t)(CCU8_CC8_TCCLR_TCC_Msk);

	/* Use IN_H for Event 0 on Rising edge
	 * Set start on EVENT 0 on Rising edge
	 */
	CCU80_CC83->INS |= CCU8_IN_H << CCU8_CC8_INS_EV0IS_Pos;
	CCU80_CC83->INS |= ACTIVE_ON_RISING_EDGE << CCU8_CC8_INS_EV0EM_Pos;
	CCU80_CC83->CMC |= ENABLE << CCU8_CC8_CMC_STRTS_Pos;

	//	/*Configure the timer CCU8 to generate  a service request when matching compare channel 1 while counting down ON EVENT 2*/
	//	CCU80_CC83->INS  |=  15 << CCU8_CC8_INS_EV2IS_Pos;   		//which input is used -> input CC80.IN3P
	//	CCU80_CC83->INS  &=  ~CCU8_CC8_INS_EV2EM_Msk;	 			// clear event active on rising edge
	//	CCU80_CC83->INS  |=  ENABLE  << CCU8_CC8_INS_EV2EM_Pos;	 	// set event active on rising edge

	/*Enables the Channel 1 compare match while counting down interrupt*/
	CCU80_CC83->INTE |= ENABLE << CCU8_CC8_INTE_CMD1E_Pos;

	/*The interrupt generated by compare match of channel 1 is forwarded to CC8ySR3*/
	CCU80_CC83->SRS |= CCU8_SRS_CM1SR_SR3_MSK << CCU8_SRS_CM1SR_POS;

	/*Enable the VADC G0 Line 2 interrupt*/
	NVIC_ClearPendingIRQ(VADC0_G0_2_IRQn);
	NVIC_EnableIRQ(VADC0_G0_2_IRQn);

	/*Set priority level for VADC to 1(second highest)*/
	NVIC_SetPriority(VADC0_G0_2_IRQn, 1U);

	//	/*Enable the VADC G2 Line 2 interrupt*/
	//	NVIC_ClearPendingIRQ(VADC0_G2_2_IRQn);
	//	NVIC_EnableIRQ(VADC0_G2_2_IRQn);
	//
	//	/*Set priority level for VADC to 1(second highest)*/
	//	NVIC_SetPriority(VADC0_G2_2_IRQn, 10U);

	SCU_GENERAL->CCUCON |= SCU_GENERAL_CCUCON_GSC80_Msk;
	SCU_GENERAL->CCUCON &= ~SCU_GENERAL_CCUCON_GSC80_Msk;
}

/*
	Read currents from VADC
	abc RES[1] | u_mot u_log RES[14]
	'a' is too noisy => computed form 'bc'
*/
void read_currents(void)
{
	ia = (VADC_G0->RES[1] & 0xFFFF) * 16 - offset_ia; // 16 bit value -- Very noisy
	ib = (VADC_G1->RES[1] & 0xFFFF) * 16 - offset_ib; // 16 bit value
	ic = (VADC_G2->RES[1] & 0xFFFF) * 16 - offset_ic; // 16 bit value
	ia = -ib;										  // use ib instead of ia

	u_mot_dig = (VADC_G2->RES[14] & 0xFFFF); // 12 bit value
	u_log_dig = (VADC_G1->RES[14] & 0xFFFF); // 12 bit value

}
/**
 * Compute current offset for every phase
 */
void current_offset(void)
{
	prot_status = OFF; //Turn Current protection OFF
	offset_ia = 0;
	offset_ib = 0;
	offset_ic = 0;

	for (int i = 0; i < 16; ++i)
	{
		delay_t(3);
		offset_ia += ((VADC_G0->RES[1] & 0xFFFF) * 16);
		offset_ib += ((VADC_G1->RES[1] & 0xFFFF) * 16);
		offset_ic += ((VADC_G2->RES[1] & 0xFFFF) * 16);
	}

	offset_ia = offset_ia / 16;
	offset_ib = offset_ib / 16;
	offset_ic = offset_ic / 16;
	prot_status = ON; //Turn Current protection ON
}

/*
 * For Micrium display
 * Compute 12bit value of u_mot into volts
 * u_mot_max = 100V
 */
void compute_u_mot(void)
{
	u_mot = u_mot_dig * 0.0245;
}

/*
 * For Micrium display
 * Compute the 12bit value of u_log into volts
 * u_log_max = 50V
 */
void compute_u_log(void)
{
	u_log = u_log_dig * 0.01327;
}

/*
 * For Micrium read
 * I_pos = 40A | I_neg = -40A
 */
void compute_currents(void)
{
	ia_a = (float_t)(ia + 32768) / 65535.0 * 80.0 - 40.0;
	ib_a = (float_t)(ib + 32768) / 65535.0 * 80.0 - 40.0;
	ic_a = (float_t)(ic + 32768) / 65535.0 * 80.0 - 40.0;
	iq_a = (float_t)(i_q + 32768) / 65535.0 * 80.0 - 40.0;
}
