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
uint16_t timer_diff;

/**
 * Initialize position for motor
 * Enable PWM output
 * B High -> wait -> A High 0 -> wait
 * Reset: Theta | Position | Timers_old
 * Disable PWM output
 */
void init_position(void) // TO DO:check if it still works
{
	motion_on();		   // Enable PWM pins
	pwm_update(0, 1000);   // Apply voltage on B Phase
	delay_t(SECOND_1 / 2); // Wait 0.5s

	pwm_update(1000, 0);   // Apply voltage on A Phase
	delay_t(SECOND_1 / 2); // Wait 0.5s

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
//	motor_spd = timer_value_old_slow - timer_value_slow;

	if (timer_value_slow >= timer_value_old_slow)
	{
		timer_diff = timer_value_slow - timer_value_old_slow;
		if (timer_diff > (UINT16_MAX / 2))
		{
			motor_spd = (int16_t)(-((UINT16_MAX - timer_diff) + 1));
		}
		else
		{
			motor_spd = (int16_t)timer_diff;
		}
	}
	else
	{
		timer_diff = timer_value_old_slow - timer_value_slow;
		if (timer_diff > (UINT16_MAX / 2))
		{
			motor_spd = (int16_t)((UINT16_MAX - timer_diff) + 1);
		}
		else
		{
			motor_spd = (int16_t)(-timer_diff);
		}
	}

	timer_value_old_slow = timer_value_slow;
}
