#include <stdint.h>
#include <stdbool.h>
#include <xmc_common.h>
#include "driver_pwm.h"
#include "motor_data_update.h"
#include "transform.h"
#include "main.h"
#include <math.h>
#include "Encoder.h"
#include "driver_adc.h"
#include "pi_regulator.h"
#include "state_machine.h"

/*
 * Variables
 */
int32_t mechanical_position_fast = 0;			//compute the mechanical position of the motor based on speed increments
int32_t electrical_position_modulo_fast = 0;	//the module of electrical position based on electrical resolution and mechanical position
float theta_fast = 0;							//electric angle
float sin_theta_fast = 0;						//sin(theta)
float cos_theta_fast = 0;						//cos(theta)
int16_t timer_value_fast = 0;					//Stored value of CCU40 timer for encoder
int16_t timer_value_old_fast = 0;				//*Last* read of timer value for speed computation
int16_t speed_value_fast = 0;					//Speed value based on 2 timer values
int16_t encoder_resolution = 2000;				//Resolution of encoder | 4*500
int8_t pp = 4;									//Number of electric poles
int16_t electrical_resolution = 0;				//Electrical resolution based on encoder resolution and number of electric poles | 2000/4 = 500
float u_a_ref,u_b_ref,u_c_ref;					//abc voltage | -32768 -- 32,767
float i_d,i_q; 									//dq voltage																   *
float i_alpha, i_beta;							//currents for dq computation
float u_alpha, u_beta;							//voltage for abc computation

/*
 * Function to convert three-phase electrical signals from time-domain to dq0-reference frame
 */
void abc_dq (void)
{	
	 //The Clarke Transformation
	i_alpha = ia;
	i_beta = 1/sqrtf(3) * (float)(ia + (float)(2*ib));

	//The Park Transformation
	i_d = i_alpha*cos_theta_fast + i_beta*sin_theta_fast;
	i_q = i_beta*cos_theta_fast - i_alpha*sin_theta_fast;

}

/*
 * Function to convert two-phase balanced sinusoidal signals to three-phase electrical signals
 */
void dq_abc(void)
{

 	//The Inverse Park Transformation
	u_alpha = u_d_ref*cos_theta_fast - u_q_ref*sin_theta_fast;
	u_beta = u_q_ref*cos_theta_fast + u_d_ref*sin_theta_fast;

	//The Inverse Clarke Transformation
	u_a_ref = u_alpha;
	u_b_ref = (-u_alpha+sqrtf(3)*u_beta)/2;
	u_c_ref = (-u_alpha-sqrtf(3)*u_beta)/2;

	/*
	 * Keeps the values in range of int16_t
	 * Later on the Float values will be casted into Int
	 */
	if (u_a_ref > MAX_UINT_16)
	{
		u_a_ref = MAX_UINT_16;
	}
	else if (u_a_ref < MIN_UINT_16)
	{
		u_a_ref = MIN_UINT_16;
	}

	if (u_b_ref > MAX_UINT_16)
	{
		u_b_ref = MAX_UINT_16;
	}
	else if (u_b_ref < MIN_UINT_16)
	{
		u_a_ref = MIN_UINT_16;
	}

	if (u_c_ref > MAX_UINT_16)
	{
		u_a_ref = MAX_UINT_16;
	}
	else if (u_c_ref < MIN_UINT_16)
	{
		u_c_ref = MIN_UINT_16;
	}

}

/*
 * Compute the speed based on the encoder increments, read from CCU40 Timer
 */
void compute_fast_speed (void)
{
	timer_value_fast = CCU40_CC40->TIMER;
	speed_value_fast = timer_value_fast - timer_value_old_fast;
	//FOR TESTING I SET A CONSTANT SPEED VALUE
//	speed_value_fast = -1;
	timer_value_old_fast = timer_value_fast;

}
/*
 * Compute the mechanical position based on the speed value
 */
void compute_fast_mechanical_position (void)
{
	mechanical_position_fast += speed_value_fast;
}

/*
 * Compute the module of the electrical position based on the mechanical position
 */
void compute_fast_electrical_position (void)
{
	electrical_resolution = encoder_resolution/pp;
	electrical_position_modulo_fast = mechanical_position_fast % electrical_resolution;
}

/*
 * Compute the electrical angle based on the electrical position
 * Compute the sin&cos of theta
 */
void compute_fast_field (void)
{
	theta_fast = electrical_position_modulo_fast*(2*Pi)/electrical_resolution;
	sincosf(theta_fast, &sin_theta_fast, &cos_theta_fast);
	if(tune_test_control)
	{
		theta_fast = 0;
	}
}

// void abc_dq_test (void)		//TEST
// {
// 	/*
// 	 * The Clarke Transformation
// 	 */
// 	i_alpha = signal_ia_test;
// 	i_beta = 1/sqrtf(3) * (float)(signal_ia_test + (2.*(float)signal_ib_test));
// 	/*
// 	 * The Park Transformation
// 	 */
// 	i_d = i_alpha*cos_increment_2_pi + i_beta*sin_increment_2_pi;
// 	i_q = i_beta*cos_increment_2_pi - i_alpha*sin_increment_2_pi;

// }
