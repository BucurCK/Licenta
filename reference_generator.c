#include <stdint.h>
#include <stdbool.h>
#include "reference_generator.h"
#include "driver_adc.h"
#include "pi_regulator.h"
#include "state_machine.h"
#include "transform.h"
#include <math.h>

// For reference Time = [ms] | high_level = motor counts
uint32_t interrupt_counter_ref_gen = 0;

status_ref_gen ref_gen_status = 0; // 0 - disabled, 1 - rise, 2 - high, 3 - fall, 4 - low

int32_t high_level;		 // final reference
int32_t high_level_time; // time to hold final reference

int32_t rise_time; // time to get to final reference
float_t rise_increment;

int32_t fall_time; // time to get to initial reference
float_t fall_decrement;

int32_t low_level;		// initial reference = 0
int32_t low_level_time; // time to hold initial reference

uint8_t repeat_motion = 1; // 0 = NO | 1 = YES

float_t time_rise_input_s = 0;
float_t time_fall_input_s = 0;
float_t time_high_input_s = 0;
float_t time_low_input_s = 0;

float_t current_input = 0; // 1[A] ~= 820[i.u]
float_t voltage_input = 0;
float_t rpm_input = 0;		// rpm_min = 30 = 1[iu/ms] | use increments of 30 for use of computation
float_t rotation_input = 0; // number of full rotation | 1[rot] = 2000[iu]

float_t reference = 0;
float_t reference_old = 0;

ref_type ref_type_select = 0;

void reference_generator_compute(void)
{
	// Compute human data inputs into machine outputs
	switch (ref_type_select)
	{
	case (REF_POS):
		high_level = rotation_input * ROTATION_IU;
		high_level_time = time_high_input_s * SECOND;
		low_level_time = time_low_input_s * SECOND;
		rise_time = time_rise_input_s * SECOND;
		fall_time = time_fall_input_s * SECOND;

//		high_level = 1 * ROTATION_IU;
//		high_level_time = 0.1 * SECOND;
//		low_level_time = 0.1 * SECOND;
//		rise_time = 0.2 * SECOND;
//		fall_time = 0.2 * SECOND;

		break;
	case (REF_SPD):
		rise_time = time_rise_input_s * SECOND;
		fall_time = time_high_input_s * SECOND;
		high_level = rpm_input / (MINUTE / ROTATION_IU);
		high_level_time = time_high_input_s * SECOND;
		low_level_time = time_low_input_s * SECOND;

//		rise_time = 0.5 * SECOND;
//		fall_time = 0.5 * SECOND;
////		high_level = rpm_input / (MINUTE / ROTATION_IU);
//		high_level_time = 0.5 * SECOND;
//		low_level_time = 0.5 * SECOND;
		break;
	case (REF_I):
		high_level = current_input;
		high_level_time = time_high_input_s * SECOND;
		low_level_time = time_low_input_s * SECOND;
		rise_time = time_rise_input_s * SECOND;
		fall_time = time_fall_input_s * SECOND;

//		high_level = 500;
//		high_level_time = 0.5 * SECOND;
//		low_level_time = 0.5 * SECOND;
//		rise_time = 0.01 * SECOND;
//		fall_time = 0.01 * SECOND;
		break;
	case (REF_U):
		high_level = voltage_input;
		high_level_time = time_high_input_s * SECOND;
		low_level_time = time_low_input_s * SECOND;
		rise_time = time_rise_input_s * SECOND;
		fall_time = time_fall_input_s * SECOND;

//		high_level = 2000;
//		high_level_time = 5 * SECOND;
//		low_level_time = 0 * SECOND;
//		rise_time = 0 * SECOND;
//		fall_time = 0 * SECOND;
		break;
	default:
		break;
	}

	//No rise time
	if (0 == rise_time)
	{
		rise_increment = high_level;
	}
	else
	{
		rise_increment = (float_t)high_level / rise_time;
	}

	//No fall time
	if (0 == fall_time)
	{
		fall_decrement = high_level;
	}
	else
	{
		fall_decrement = (float_t)high_level / fall_time;
	}

	reference = 0;
	reference_old = 0;

	__disable_irq();
	interrupt_counter_ref_gen = 0;
	__enable_irq();
	ref_gen_status = STATUS_1_RISE;
}

void reference_generator(void)
{
	switch (ref_gen_status)
	{
	case (STATUS_1_RISE):
		if (interrupt_counter_ref_gen >= rise_time)
		{
			ref_gen_status = STATUS_2_HIGH_LEVEL;
			interrupt_counter_ref_gen = 0;
		}
		reference += rise_increment;

		break;

	case (STATUS_2_HIGH_LEVEL):
		if (interrupt_counter_ref_gen >= high_level_time)
		{
			ref_gen_status = STATUS_3_FALL;
			interrupt_counter_ref_gen = 0;
		}
		break;

	case (STATUS_3_FALL):
	{
		if (interrupt_counter_ref_gen >= fall_time)
		{
			ref_gen_status = STATUS_4_LOW_LEVEL;
			interrupt_counter_ref_gen = 0;
		}
		reference -= fall_decrement;
	}
	break;

	case (STATUS_4_LOW_LEVEL):
		if ((interrupt_counter_ref_gen >= low_level_time) && (repeat_motion == 0))
		{
			ref_gen_status = STATUS_0_DISABLED;
			interrupt_counter_ref_gen = 0;
		}
		else if ((interrupt_counter_ref_gen >= low_level_time) && (repeat_motion == 1))
		{
			ref_gen_status = STATUS_1_RISE;
			interrupt_counter_ref_gen = 0;
		}
		break;

	default:
		break;
	}

	switch (ref_type_select)
	{
	case (REF_POS):
		pos_ref = reference;
		break;
	case (REF_SPD):
		spd_ref = reference;
		break;
	case (REF_I):
		i_q_ref = reference;
		break;
	case (REF_U):
		u_q_ref = reference;
		break;
	default:
		break;
	}
}
