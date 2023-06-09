#include <stdint.h>
#include <stdbool.h>
// #include <xmc_common.h>
#include "driver_pwm.h"
#include "motor_data_update.h"
#include "transform.h"
#include "main.h"
#include <math.h>
#include "driver_pwm.h"
#include "timer.h"
#include "driver_adc.h"

int32_t motor_spd = 0;
uint16_t timer_value_slow = 0;
uint16_t timer_value_old_slow = 0;

/**
 * Initialize position for motor
 * Enable PWM output
 * B High -> wait -> A High 0 -> wait
 * Reset: Theta | Position | Timers_old
 * Disable PWM output
 */
void init_position(void)
{
	motion_on();			// Enable PWM pins
	pwm_update(0, 4000, 0); // Apply voltage on B Phase
	delay_t(SECOND_1 / 2);	// Wait 0.5s

	pwm_update(4000, 0, 0); // Apply voltage on A Phase
	delay_t(SECOND_1 / 2);	// Wait 0.5s
//	motion_off();			// Disable PWM pin			NEED TO CHECK if it still works

	theta_fast = 0;
	mechanical_position_fast = 0;
	timer_value_old_fast = CCU40_CC40->TIMER;
	timer_value_old_slow = CCU40_CC40->TIMER;
}

/*
 * Compute current speed
 * Slow loop
 */
void compute_speed(void)
{
	timer_value_slow = CCU40_CC40->TIMER;
	motor_spd = timer_value_slow - timer_value_old_slow;
	timer_value_old_slow = timer_value_slow;
}
