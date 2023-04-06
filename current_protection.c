/*
 * current_protection.c
 *
 *  Created on: 6 Apr 2023
 *      Author: xraid
 */
#include "main.h"
#include "current_protection.h"
#include "transform.h"
#include "state_machine.h"
#include "driver_pwm.h"

float i_prot = 2 * 820.;						// Ampers converted into 16bit scale | 32768/40 ~= 820
float k = 0.5;									// Percentage of filtered signal
float filter_output = 0, filter_output_old = 0;	//Output of filtered signal | Last output of filtered signal
uint32_t counter_timer_prot = 0;				//Timer increment in 100us -- if functione called in fast_loop

/*
*	y = k*(x - y_last) + y_last
*	First degree signal filter
*/
void current_filter (float filter_input)
{
	filter_output = k * (filter_input - filter_output_old) + filter_output_old;
	filter_output_old = filter_output;
}

/*
*	Stops current output if above i_prot for TIME_TRIGGER_FAIL time
*/
void current_protection (void)
{

	current_filter(i_alpha*i_alpha + i_beta*i_beta);
	if (filter_output > i_prot*i_prot)
	{
		counter_timer_prot++;

		if(counter_timer_prot > TIME_TRIGGER_FAIL)
		{

		drive_disabled();								//BUG: driver_command_old won't change
		drive_status |= DRIVE_STATUS_PROT_MSK;			//set current_prot_flag
		drive_status &= CLEAR_DRIVE_STATUS;				//set STATUS bits to 0 -> go to state 0
		counter_timer_prot = 0;							

		}
	}else
	{
		counter_timer_prot = 0;							
	}
}



