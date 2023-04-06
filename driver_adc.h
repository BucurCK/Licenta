/*
 * driver_pwm.h
 *
 *  Created on: Jul 23, 2019
 *      Author: student
 */
#include "main.h"
#ifndef DRIVER_ADC_H_
#define DRIVER_ADC_H_

extern void adc_init (void);
extern void read_currents (void);
extern void interrupt_vadc_init(void);
extern void compute_u_mot(void);
extern void compute_u_log(void);
extern void compute_currents(void);
extern void current_offset(void);

extern int16_t ia, ib, ic;

//extern float ia_a, ib_a, ic_a;

//extern int16_t ia_test, ib_test; //testing
//extern float signal_ia_test, signal_ib_test, sin_increment_2_pi, cos_increment_2_pi; //For testing


#define PORT14_IOCR0_PC0_POS			(3UL)
#define PORT14_IOCR0_PC3_POS			(27UL)
#define PORT14_IOCR4_PC4_POS			(3UL)
#define INPUT_PULL_UP_MSK				(0x2UL)
#define SCU_CGATCLR0_VADC_MSK			(0x1UL)
#define SCU_PRCLR0_VADCRS_MSK			(0x1UL)
#define VADC_CLC_DISR_MSK				(0x1UL)
#define VADC_GLOBCGF_DIVA_POS			(0UL)
#define VADC_GLOBCGF_DIVA_4_MSK			(0x3UL)
#define VADC_GLOBICLASS0_CMS_POS		(8UL)
#define VADC_GLOBICLASS0_CMS_12_MSK		(0x7UL)			//needs to be negated to 000b
#define VADC_GLOBICLASS0_STCS_POS		(0UL)
#define VADC_GLOBICLASS0_STCS_5_MSK		(0x5UL)
#define VADC_GLOBCFG_SUCAL_POS			(31UL)
#define VADC_ARBCFG_ANONC_MSK			(0x3UL)
#define VADC_ARBCFG_CAL_MSK				(0x10000000UL)
#define VADC_ARBPR_ASEN0_POS			(24UL)			//negate it to disable Arbitration
#define VADC_ARBPR_ASEN1_POS			(25UL)			//negate it to disable Arbitration
#define VADC_ARBPR_ASEN2_POS			(26UL)			//negate it to disable Arbitration
#define VADC_ARBPR_PRIO0_MAX_MSK		(0x3UL)
#define VADC_QCTRL0_TMWC_POS			(31UL)
#define VADC_QCTRL0_TMEN_POS			(28UL)
#define VADC_QCTRL0_XTWC_POS			(15UL)
#define VADC_QCTRL0_XTMODE_ANY_MSK		(0x3UL)
#define VADC_QCTRL0_XTMODE_POS			(13UL)
#define VADC_QCTRL0_XTSEL_POS			(8UL)
#define VADC_QCTRL0_XTSEL_J_MSK			(0x9UL)
#define VADC_QMR0_ENGT_MSK				(0x1UL)
#define VADC_QMR0_ENTR_MSK				(0x4UL)
#define VADC_QCTRL0_GTWC_MSK			(0x800000UL)
#define VADC_QCTRL0_GTSEL_POS			???????
#define VADC_QCTRL0_GTSEL_POS			???????
#define VADC_ARBPR_CSM0_MSK				(0x8UL)
#define VADC_CHASS_ASSCH0_MSK			(0x1UL)
#define VADC_CHASS_ASSCH6_POS			(6UL)
#define VADC_ALIAS_ALIAS0_POS			(0UL)
#define VADC_CHCTR_SYNC_MSK				(0x400UL)
#define VADC_CHCTR_ICSEL_GLOBAL_0_MSK	(0x2UL)
#define VADC_CHCTR_ICSEL_GROUP_0_MSK	(0x3UL)		//negate it
#define VADC_QINR0_RF_MSK				(0x20UL)
#define VADC_QINR0_ENSI_MSK				(0x40UL)
#define VADC_QINR0_EXTR_MSK				(0x80UL)
#define VADC_SYNCTR_EVALR1_MSK			(0x10UL)
#define VADC_SYNCTR_STSEL_CL1_MSK		(0x1UL)
#define VADC_GxRCRy_SRGEN_MSK			(0x80000000UL)
#define CCU8_SRS_CM1SR_POS				(2UL)
#define CCU8_SRS_CM1SR_SR3_MSK			(0x3UL)
#define VADC_REVNP0_REV0NP_POS			(0UL)
#define VADC_REVNP0_REV2NP_POS			(8UL)
#define VADC_REVNP0_REV0NP_L2G0_MSK		(0x2UL)
#define ENABLE							(0x1UL)
#define DISABLE							(0x1UL)
#define VADC_ASSEL_CHSEL6_POS			(6UL)
#define VADC_ASMR_SCAN_MSK				(0x10UL)
#define VADC_ARBPR_ASEN1_POS			(25UL)
#define VADC_ASMR_ENGT_MSK				(0x1UL)
#define VADC_ASMR_LDEV_MSK				(0x200UL)
#define VADC_GLOBCFG_DIVWC				(0x8000UL)

#define GENERAL_OUTPUT					(0x10UL)



#endif /* DRIVER_PWM_H_ */
