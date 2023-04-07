#include <stdint.h>
#include <stdbool.h>
#include "reference_generator.h"
#include "driver_adc.h"
#include "pi_regulator.h"
#include "state_machine.h"
#include "transform.h"

//For reference
uint32_t interrupt_counter_ref_gen = 0;
status_ref_gen ref_gen_status = 0;					//0 - disabled, 1 - rise, 2 - high, 3 - fall, 4 - low
int32_t high_level = 2000;							//final reference
int32_t high_level_time = 1000;					//time to hold final reference

int32_t rise_time = 2000;							//time to get to final reference
int32_t rise_increment;

int32_t fall_time = 2000;							//time to get to initial reference
int32_t fall_decrement;

int32_t low_level = 0;								//initial reference = 0
int32_t low_level_time = 1000;						//time to hold initial reference

uint8_t repeat_motion = 1;							// 0 = NO | 1 = YES

float reference = 0;
float reference_old = 0;

ref_type ref_type_select = 0;


void reference_generator_compute (void)
{
	rise_increment = high_level/rise_time;
	fall_decrement = high_level/fall_time;
	//************************************* Move it to drive_commands
	__disable_irq();
	interrupt_counter_ref_gen = 0;
	__enable_irq();
	ref_gen_status = STATUS_1_RISE;
	//*************************************
}

void reference_generator (void)
{
	switch(ref_gen_status)
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
			if (interrupt_counter_ref_gen >= fall_time)
			{
				ref_gen_status = STATUS_4_LOW_LEVEL;
				interrupt_counter_ref_gen = 0;
			}
				reference -= fall_decrement;

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
	case(REF_POS):
			pos_ref = reference;
			break;
	case(REF_SPD):
			spd_ref = reference - reference_old;
			reference_old = reference;
			break;
	case(REF_I):
			i_q_ref = reference;
			break;
	case(REF_U):
			u_q_ref = reference;
			break;
	default:
		break;
	}

}

