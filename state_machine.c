/*
 * state_machine.c
 *
 *  Created on: 24 Mar 2023
 *      Author: xraid
 */
#include "main.h"
#include "state_machine.h"
#include "driver_adc.h"
#include "driver_pwm.h"
#include "motor_data_update.h"
#include "reference_generator.h"
#include "transform.h"

uint16_t drive_status = STATE_0_DRIVE_DISABLED;
uint16_t drive_command = STATE_0_DRIVE_DISABLED;
uint16_t drive_command_old = STATE_0_DRIVE_DISABLED;
uint8_t loop_control = LOOP_CONTROL_OFF;

/* 0 => do not compute any motion operations
   1 => compute motion operations*/
uint8_t motion_config = OFF;

/*
	Simple State Machine
	DRIVE_DISABLED | DRIVE_ON | OPERATION_ENABLED
*/
void state_machine(void)
{
	switch (DRIVE_STATUS_MSK)
	{
	case (STATE_0_DRIVE_DISABLED):
		// Transition 0->1
		if ((DRIVE_COMMAND_OLD_STATE_MSK == STATE_0_DRIVE_DISABLED) && (DRIVE_COMMAND_STATE_MSK == STATE_1_DRIVE_ON))
		{
			drive_on();
			drive_status &= DRIVE_STATUS_CLEAR;
			drive_status |= STATE_1_DRIVE_ON;
			drive_command_old = drive_command;
		}
		break;

	case (STATE_1_DRIVE_ON):
		// Transition 1->2
		if ((DRIVE_COMMAND_OLD_STATE_MSK == STATE_1_DRIVE_ON) && (DRIVE_COMMAND_STATE_MSK == STATE_2_OPERATION_ENABLED))
		{
			operation_enabled();
			drive_status &= DRIVE_STATUS_CLEAR;
			drive_status |= STATE_2_OPERATION_ENABLED;
			drive_command_old = drive_command;
		}
		// Transition 1->0
		if ((DRIVE_COMMAND_OLD_STATE_MSK == STATE_1_DRIVE_ON) && (DRIVE_COMMAND_STATE_MSK == STATE_0_DRIVE_DISABLED))
		{
			drive_disabled();
			drive_status &= DRIVE_STATUS_CLEAR;
			drive_status |= STATE_0_DRIVE_DISABLED;
			drive_command_old = drive_command;
		}
		break;

	case (STATE_2_OPERATION_ENABLED):
	{
		// Transition 2->1
		if ((DRIVE_COMMAND_OLD_STATE_MSK == STATE_2_OPERATION_ENABLED) && (DRIVE_COMMAND_STATE_MSK == STATE_1_DRIVE_ON))
		{
			drive_on();
			drive_status &= DRIVE_STATUS_CLEAR;
			drive_status |= STATE_1_DRIVE_ON;
			drive_command_old = drive_command;
		}
		// Transition 2->0
		if ((DRIVE_COMMAND_OLD_STATE_MSK == STATE_2_OPERATION_ENABLED) && (DRIVE_COMMAND_STATE_MSK == STATE_0_DRIVE_DISABLED))
		{
			drive_disabled();
			drive_status &= DRIVE_STATUS_CLEAR;
			drive_status |= STATE_0_DRIVE_DISABLED;
			drive_command_old = drive_command;
		}
		// Check for update transition and OPERATION_ENABLED still active
		if ((!(DRIVE_COMMAND_UPDATE_OLD_MSK & UPDATE_MSK) && (DRIVE_COMMAND_UPDATE_MSK & UPDATE_MSK)) && (DRIVE_COMMAND_STATE_MSK == STATE_2_OPERATION_ENABLED))
		{
			loop_control = LOOP_CONTROL_OFF;			// reset all loops
			loop_control |= DRIVE_COMMAND_REF_MSK >> 8; // save selected loops

			// Check for reference generator output (Priority list: POS > SPD > I > U)
			if (DRIVE_COMMAND_REF_MSK & REF_POS_MSK)
			{
				// POS REF
				ref_type_select = REF_POS;
			}
			else if (DRIVE_COMMAND_REF_MSK & REF_SPD_MSK)
			{
				// SPD REF
				ref_type_select = REF_SPD;
			}
			else if (DRIVE_COMMAND_REF_MSK & REF_I_MSK)
			{
				// I REFF
				ref_type_select = REF_I;
				if (DRIVE_COMMAND_TEST_MSK & TUNE_TEST)
				{
					loop_control |= TUNE_REF_LOOP_MSK;
				}
			}
			else
			{
				// U REF
				ref_type_select = REF_U;
			}
			compute_motion();
			drive_command_old = drive_command;
		}
	}
	break;
	default:
		break;
	}
}
/*
	PWM output disabled
	CONTROL_LOOPS | REF_GEN | MOTION = OFF
*/
void drive_disabled(void)
{
	motion_off();
	loop_control = LOOP_CONTROL_OFF;
	ref_gen_status = STATUS_0_DISABLED;
	motion_config = OFF;
}
/*
	Initilize position | PWM(50%) | PWM output enabled | Current offset computed
	CONTROL_LOOPS | REF_GEN | MOTION | = OFF
*/
void drive_on(void)
{
	motion_off();
	loop_control = LOOP_CONTROL_OFF;
	ref_gen_status = STATUS_0_DISABLED;
	motion_config = OFF;

	pwm_update(0, 0); // PWM(50%)
	motion_on();
	current_offset();
	init_position();		//NEED TO CHECK if it still works !!!!
}

/*
	CONTROL_LOOPS = OFF
	Wait for motion commands to start movement computation
*/
void operation_enabled(void)
{
	loop_control = LOOP_CONTROL_OFF;
}

/*
	During OPERATION_ENABLE start computation function
*/
void compute_motion(void)
{
	reference_generator_compute();
	motion_config = ON;
}
