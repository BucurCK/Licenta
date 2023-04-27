#include <stdint.h>
#include <stdbool.h>
#include <xmc_common.h> // includes stdint stdbool stdef stdstring
#include "driver_pwm.h"
#include "main.h"
#include <xmc_vadc.h>
#include <xmc_ccu4.h>
#include <kernel_interrupt.h>
#include "io.h"
#include "timer.h"
#include "Encoder.h"
#include <probe_scope.h>
#include "driver_adc.h"
#include "transform.h"
#include "motor_data_update.h"
#include "state_machine.h"
#include "current_protection.h"
#include "pi_regulator.h"

void infinite_loop(void)
{
	state_machine();
}

int main(void)
{
	interrupt_vadc_init();	// Interrupts generated by the VADC - sets up the CCU80 - CC83
	pwm_init();				// PWM & clock initialization - CC80/81/82
	encoder_init();			// Encoder initialization
	adc_init();				// VADC initialization
	ProbeScope_Init(10000); // Initialize Oscilloscope for Micrium - imported
	//	encoder_interrupt_init();	// Interrupt generated by Encoder
	//	init_io();					// (For testing I/O stuff) - not used anymore
	__enable_irq();			// built in interrupt enable function
	pi_init();

	while (1U)
	{
		/* Infinite loop */
		infinite_loop();
	}
}
