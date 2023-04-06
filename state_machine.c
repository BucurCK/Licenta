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

void state_machine (void)
{
	switch(drive_status & 0xF)
	{
	case (STATE_0_DRIVE_DISABLED):
		//Transition 0->1
		if ((DRIVE_COMMAND_OLD_STATE_MSK == STATE_0_DRIVE_DISABLED) && (DRIVE_COMMAND_STATE_MSK == STATE_1_DRIVE_ON))
			{
				drive_on();
				drive_status = STATE_1_DRIVE_ON;
				drive_command_old = drive_command;
			}
			break;
	case (STATE_1_DRIVE_ON):
		//Transition 1->2
		if ((DRIVE_COMMAND_OLD_STATE_MSK == STATE_1_DRIVE_ON) && (DRIVE_COMMAND_STATE_MSK == STATE_2_OPERATION_ENABLED))
			{
				operation_enabled();
				drive_status = STATE_2_OPERATION_ENABLED;
				drive_command_old = drive_command;
			}
			break;
	case (STATE_2_OPERATION_ENABLED):
	{
		//Transition 2->1
		if ((DRIVE_COMMAND_OLD_STATE_MSK == STATE_2_OPERATION_ENABLED) && (DRIVE_COMMAND_STATE_MSK == STATE_1_DRIVE_ON))
			{
				drive_on();
				drive_status = STATE_1_DRIVE_ON;
				drive_command_old = drive_command;
			}
		//Transition 2->0
		if ((DRIVE_COMMAND_OLD_STATE_MSK == STATE_2_OPERATION_ENABLED) && (DRIVE_COMMAND_STATE_MSK == STATE_0_DRIVE_DISABLED))
			{
				drive_disabled();
				drive_status = STATE_0_DRIVE_DISABLED;
				drive_command_old = drive_command;
			}
		//Check for update
		if(!(DRIVE_COMMAND_UPDATE_OLD_MSK & UPDATE_MSK) && (DRIVE_COMMAND_UPDATE_MSK & UPDATE_MSK))
		{
			if(DRIVE_COMMAND_REF_MSK & REF_POS_MSK)
			{
				//POS REF
				ref_type_select = REF_POS;
			}
			else if(DRIVE_COMMAND_REF_MSK & REF_SPD_MSK)
			{
				//SPD REF
				ref_type_select = REF_SPD;
			}
			else if(DRIVE_COMMAND_REF_MSK & REF_I_MSK)
			{
				//I REFF
				ref_type_select = REF_I;
				if(DRIVE_COMMAND_TEST_MSK & TUNE_TEST)
				{
					theta_fast = 0;
					//disable theta computation
				}
			}
			else
			{
				//U REF
				ref_type_select = REF_U;
			}
			drive_command_old = drive_command;
		}
	}
			break;
	default:
		break;
	}
}

void drive_disabled (void)
{
	motion_off();
	loop_control = LOOP_CONTROL_OFF;	//control_loop_off
	ref_gen_status = STATUS_0_DISABLED;
}

void drive_on (void)
{
	motion_off();
	current_offset();
	init_position();
	pwm_update(0, 0, 0);				//pwm_on_50%
	loop_control = LOOP_CONTROL_OFF;	//control_loop_off
	ref_gen_status = STATUS_0_DISABLED;
}

void operation_enabled (void)
{
	loop_control = LOOP_CONTROL_ON;		//control_loop_on
	motion_on();
	reference_generator_compute();		//TESTING
}

