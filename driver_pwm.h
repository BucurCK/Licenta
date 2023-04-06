/*
 * driver_pwm.h
 *
 *  Created on: Jul 23, 2019
 *      Author: student
 */
#include "main.h"

#ifndef DRIVER_PWM_H_
#define DRIVER_PWM_H_

extern void posif_init_encoder (void);
extern void posif_init_hall (void);
extern void pwm_init (void);
extern void pwm_update (int16_t ,int16_t ,int16_t);
extern void pwm_timer_start (void);
extern void pwm_timer_stop (void);
extern void motion_on(void);
extern void motion_off(void);


#define CCU8_CC8_PRS_PR_VALUE 	(3600UL)    // 20khz
#define CCU8_CC8_CR1_CR1_VALUE 	(1800UL)    // 10khz
#define CCU8_CC8_DC1R_Value		(8UL)	  // 100ns = 10Mhz = 14.4/2 = 7.2 ->gresit la mine ca am pus doar 50ns pe rising
#define GPIO_OUTPUT_ALTER_3		(0x13UL)
#define PORT5_PC11_POS			(27UL)
#define PORT5_PC8_POS			(3UL)
#define PORT5_PC10_POS			(19UL)
#define PORT5_PC9_POS			(11UL)
#define PORT2_PC14_POS			(19UL)
#define PORT2_PC15_POS			(27UL)
#define SCU_PRSET0_CCU80RS_MSK	(0x80UL)
#define SCU_PRCLR0_CCU80RS_MSK	(0x80UL)
#define SCU_CLKSET_CCUCEN_MSK	(0x10UL)
#define CCU8_GIDLC_SPRB_MSK		(0x100UL)
#define CCU8_GCSS_S0SE_MSK		(0x1UL)
#define CCU8_GCSS_S1SE_MSK		(0x10UL)
#define CCU8_GCSS_S2SE_MSK		(0x100UL)
#define CCU8_GCSS_S3SE_MSK		(0x1000UL)
#define CCU8_CC8_TC_TCM_MSK		(0x1UL)
#define CCU8_GIDLC_CS0I_MSK		(0x1UL)
#define CCU8_GIDLC_CS1I_MSK		(0x2UL)
#define CCU8_GIDLC_CS2I_MSK		(0x4UL)
#define CCU8_GIDLC_CS3I_MSK		(0x8UL)
#define CCU8_CC8_TCCLR_TRBC_MSK	(0x1UL)
#define CCU8_CC8_TCCLR_TCC_MSK	(0x2UL)
#define CCU8_CC8_DTC_DTE1_MSK	(0x1UL)
#define CCU8_CC8_DTC_DCEN1_MSK	(0x4UL)
#define CCU8_CC8_DC1R_DT1R_POS	(0UL)
#define CCU8_IN_H				(7UL)
#define CCU8_IN_A				(0UL)
#define	RISING_EDGE				(1UL)
#define BOTH_EDGE				(0x3UL)
#define FALLING_EDGE			(0x2UL)
#define CCU8_CC8_INS_EV0IS_POS	(0UL)
#define CCU8_CC8_INS_EV1IS_POS	(4UL)
#define CCU8_CC8_INS_EV0EM_POS	(16UL)
#define CCU8_CC8_INS_EV1EM_POS	(18UL)
#define CCU8_CC8_CMC_STRTS_POS	(0UL)
#define CCU8_CC8_CMC_ENDS_POS	(2UL)
#define CCU8_CC8_CMC_STRTS_EV0_MSK		(1UL)
#define CCU8_CC8_CMC_ENDS_EV1_MSK		(0x2UL)
#define SCU_GENERAL_CCUCON_GSC80_MSK 	(0x100UL)
#define PWM_VALUE				(3600)
#define DISABLE_OUTPUT			(0x1FUL)


#endif /* DRIVER_PWM_H_ */
