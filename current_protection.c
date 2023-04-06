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

float i_prot = 0.2 * 820.;		// 32767/40		A->I.U
float i_prot_square;
uint32_t timer_prot = 0;
float k = 0.5, x = 0;
float filter_output = 0, filter_output_old = 0;

void current_filter (void)
{
	x = i_alpha*i_alpha + i_beta*i_beta;
	filter_output = k * (x - filter_output_old) + filter_output_old;
	filter_output_old = filter_output;
}

void current_protection (void)
{
	i_prot_square = i_prot*i_prot;
	current_filter();
	if (filter_output > i_prot*i_prot)
	{
		timer_prot++;
		if(timer_prot > TIME_TRIGGER_FAIL)
		{
		drive_disabled();
		drive_status |= DRIVE_STATUS_FAIL_MSK;
		drive_status &= ~(0xFU);
		timer_prot = 0;
		}
	}else
	{
		timer_prot = 0;
	}
}



