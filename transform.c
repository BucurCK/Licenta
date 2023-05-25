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
#include <math.h>

int32_t mechanical_position_fast = 0;		 // compute the mechanical position of the motor based on speed increments
int32_t electrical_position_modulo_fast = 0; // the module of electrical position based on electrical resolution and mechanical position
float_t theta_fast = 0;						 // electric angle
float_t sin_theta_fast = 0;					 // sin(theta)
float_t cos_theta_fast = 0;					 // cos(theta)
uint16_t timer_value_fast = 0;				 // Stored value of CCU40 timer for encoder
uint16_t timer_value_old_fast = 0;			 //*Last* read of timer value for speed computation
int16_t speed_value_fast = 0;				 // Speed value based on 2 timer values
int16_t encoder_resolution = 0;			 // Resolution of encoder | 4*resolution
int8_t pp = 0;								 // Number of electric poles : Total_steps/mechanical_steps_electric_angle
int16_t electrical_resolution = 0;			 // Electrical resolution based on encoder resolution and number of electric poles
float_t u_a_ref, u_b_ref, u_c_ref;			 // abc voltage | -32768 -- 32,767
float_t i_d, i_q;							 // dq voltage																   *
float_t i_alpha, i_beta;					 // currents for dq computation
float_t u_alpha, u_beta;					 // voltage for abc computation
/*
	Initialize motor data
	enc_res = encoder resolution
	motor_steps = number of steps
*/
void motor_data_init(int16_t enc_res, int8_t motor_steps)
{
	encoder_resolution = enc_res * 4;					//4000
	pp = motor_steps / 4;								//25
	electrical_resolution = encoder_resolution / pp;	//160
}

/*
 * Convert three-phase electrical signals from time-domain to dq0-reference frame
 */
void abc_dq(void)
{
	// Initializa alpha values
	i_alpha = ia;
	i_beta = ic;

	// Park Transformation
	i_d = i_alpha * cos_theta_fast + i_beta * sin_theta_fast;
	i_q = i_beta * cos_theta_fast - i_alpha * sin_theta_fast;
}

/*
 * Convert two-phase balanced sinusoidal signals to three-phase electrical signals
 */
void dq_abc(void)
{

	// The Inverse Park Transformation
	u_alpha = u_d_ref * cos_theta_fast - u_q_ref * sin_theta_fast;
	u_beta = u_q_ref * cos_theta_fast + u_d_ref * sin_theta_fast;

	// The Inverse Clarke Transformation
	u_a_ref = u_alpha;
	u_b_ref = u_beta;

	/*
	 * Keeps the values in range of int16_t
	 * Later on the Float values will be casted into Int
	 */
	if (u_a_ref > MAX_INT_16)
	{
		u_a_ref = MAX_INT_16;
	}
	else if (u_a_ref < MIN_UINT_16)
	{
		u_a_ref = MIN_UINT_16;
	}

	if (u_b_ref > MAX_INT_16)
	{
		u_b_ref = MAX_INT_16;
	}
	else if (u_b_ref < MIN_UINT_16)
	{
		u_a_ref = MIN_UINT_16;
	}
}

/*
 * Compute the speed based on the encoder increments, read from CCU40 Timer
 */
void compute_fast_speed(void) // SPEED = 1 for testing
{
	timer_value_fast = CCU40_CC40->TIMER;
	speed_value_fast = timer_value_fast - timer_value_old_fast;
	//	speed_value_fast = 1;
	timer_value_old_fast = timer_value_fast;
}
/*
 * Compute the mechanical position based on the speed value
 */
void compute_fast_mechanical_position(void)
{
	mechanical_position_fast += speed_value_fast;
}

/*
 * Compute the module of the electrical position based on the mechanical position
 */
void compute_fast_electrical_position(void)
{
	electrical_position_modulo_fast = mechanical_position_fast % electrical_resolution;
}

/*
 * Compute the electrical angle based on the electrical position
 * Compute the sin&cos of theta
 */
void compute_fast_field(void)
{
	theta_fast = electrical_position_modulo_fast * (2 * Pi) / electrical_resolution;
	if (loop_control & TUNE_REF_LOOP_MSK)
	{
		theta_fast = 0;
	}
	sincosf(theta_fast, &sin_theta_fast, &cos_theta_fast);
}
