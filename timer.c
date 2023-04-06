/*
 * timer.c
 *
 *  Created on: 13 Dec 2022
 *      Author: xraid
 */
#include "timer.h"
#include "main.h"
#include "driver_adc.h"

/*
 * Wait for time*50us
 */
void delay_t (uint32_t time)
{
	__disable_irq();
	interrupt_counter_timer = 0;
	__enable_irq();
	while (interrupt_counter_timer < time)
	{
		// NO_ACTION
	};
}
