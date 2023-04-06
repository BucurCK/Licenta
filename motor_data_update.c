#include <stdint.h>
#include <stdbool.h>
//#include <xmc_common.h>
#include "driver_pwm.h"
#include "motor_data_update.h"
#include "transform.h"
#include "main.h"
#include <math.h>
#include "driver_pwm.h"
#include "timer.h"
#include "driver_adc.h"

int32_t	motor_spd = 0;
int32_t	mechanical_position_old = 0;

/**
 * Initialize position for motor
 * 
 */
void init_position (void)
{
	motion_on();				//Enable PWM pins
	pwm_update(0, 4000, 0);		//Apply voltage on the B Phase
	delay_t(SECOND_1/2);		//Wait 0.5s

	pwm_update(4000, 0, 0);		//Apply voltage on the A Phase
	delay_t(SECOND_1/2);		//Wait 0.5s
	motion_off();				//Disable PWM pin

	theta_fast = 0;
	mechanical_position_fast = 0;
	motor_spd = 0;
	mechanical_position_old = 0;
}

/**
 * Compute current speed of motor
 * 
 */
void compute_speed (void)
{
	motor_spd = mechanical_position_fast - mechanical_position_old;
	mechanical_position_old = mechanical_position_fast;
}



