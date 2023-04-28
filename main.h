#include <xmc_ccu8.h>
#ifndef MAIN_H_
#define MAIN_H_

void uq_control(void);
void iq_control(void);
void speed_control(void);
void position_control(void);



#define Type_FAST 0
#define Type_SLOW 1

#define Pi     3.14159f
#define Sqr3   1.7320508f
#define f1sqr3 0.57735026f

//PIREG


#define Type_ID 		1UL
#define Type_IQ 		2UL
#define Type_SPD 		3UL
#define k32 			32768
//Current regulator ID
#define 	    PI_REG_Imax_Id_Value      	3000.f
#define			PI_REG_Outmax_Id_Value		30000.f


#define	        PI_REG_Kp_Id_Value			1.505f
#define		    PI_REG_Ki_Id_Value			0.000f

//Current regulator IQ
#define 	    PI_REG_Imax_Iq_Value      	5000.f
#define			PI_REG_Outmax_Iq_Value		30000.f


#define	        PI_REG_Kp_Iq_Value			4.055f
#define		    PI_REG_Ki_Iq_Value			0.05f

//Speed Regulator
#define			PI_REG_Imax_Spd_Value		2000.f
#define			PI_REG_Outmax_Spd_Value		10000.f

#define			PI_REG_Kp_Spd_Value			5.f
#define			PI_REG_Ki_Spd_Value			0.3f

//Position Regulator
#define 		PID_REG_Imax_Pos_Value		20.f
#define 		PID_REG_Outmax_Pos_Value	2500.f

#define 		PID_REG_Kp_Pos_Value		1.f
#define		 	PID_REG_Ki_Pos_Value		0.00005f
#define 		PID_REG_Kd_Pos_Value		120.f



//TODO	PIREG
//TODO	PIREG
//TODO	PIREF
//TODO	PIREG
//TODO	PIREG



typedef enum
{
	GPIO_DIRECT_INPUT_PULL_OFF = 0,
	GPIO_DIRECT_INPUT_PULL_DN,
	GPIO_DIRECT_INPUT_PULL_UP,
	GPIO_DIRECT_CONT_SAMPLE,
	GPIO_INVERT_INPUT_PULL_OFF,
	GPIO_INVERT_INPUT_PULL_DN,
	GPIO_INVERT_INPUT_PULL_UP,
	GPIO_INVERT_CONT_SAMPLE,
	GPIO_OUTPUT_PUSH_PULL = 16,
	GPIO_OUTPUT_ALT1,
	GPIO_OUTPUT_ALT2,
	GPIO_OUTPUT_ALT3,
	GPIO_OUTPUT_ALT4
}port_driver_mode_t;

typedef enum
{
	CCU8_IN_A= 0UL,
	CCU8_IN_B,
	CCU8_IN_C,
	CCU8_IN_D,
	CCU8_IN_E,
	CCU8_IN_F,
	CCU8_IN_G,
	CCU8_IN_H,
	CCU8_IN_I,
	CCU8_IN_J,
	CCU8_IN_K,
	CCU8_IN_L,
	CCU8_IN_M,
	CCU8_IN_N,
	CCU8_IN_O,
	CCU8_IN_P
}CCU8_CC8_INS_EV_IS;

typedef enum
{
	CCU4_IN_A= 0UL,
	CCU4_IN_B,
	CCU4_IN_C,
	CCU4_IN_D,
	CCU4_IN_E,
	CCU4_IN_F,
	CCU4_IN_G,
	CCU4_IN_H,
	CCU4_IN_I,
	CCU4_IN_J,
	CCU4_IN_K,
	CCU4_IN_L,
	CCU4_IN_M,
	CCU4_IN_N,
	CCU4_IN_O,
	CCU4_IN_P
}CCU4_CC4_INS_EV_IS;

typedef enum
{
	NO_ACTION = 0,
	ACTIVE_ON_RISING_EDGE,
	ACTIVE_ON_FALLING_EDGE,
	ACTIVE_ON_BOTH_EDGES
}CCU8_CC8_MODE;




/*   */
#define CCU8_CC8_PRS_PR_Value 			3600UL    // 20khz

//#define CCU8_CC8_DC1R_Value 			1800UL

uint32_t CCU8_CC8_CR1_CR1_Value;


#define CCU4_CC4_CR_Position_Value  	0xFFFFUL
#define CCU4_CC4_PR_Position_Period  	0xFFFFUL
#define CCU4_CC4_CR_Revolution_Value  	20UL
#define CCU4_CC4_PR_Revolution_Period  	20UL



#define pwm_saturation_level 			2500
#define pwm_period_value 				3600
#define u_ref_max_value 				32767
#define u_ref_min_value 				-32768

#define CCU80_CC83_CR_Value 			50



#endif /* MAIN_H_ */
