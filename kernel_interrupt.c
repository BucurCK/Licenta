#include <stdint.h>
#include <stdbool.h>
#include <xmc_common.h> // includes stdint stdbool stdef stdstring
#include "driver_pwm.h"
#include "main.h"
#include "motor_data_update.h"
#include "driver_adc.h"
#include "transform.h"
#include "io.h"
#include <probe_scope.h>
#include "Encoder.h"
#include "timer.h"
#include "motor_data_update.h"
#include "state_machine.h"
#include "pi_regulator.h"
#include "reference_generator.h"
#include "current_protection.h"
#include "kernel_interrupt.h"

uint16_t interrupt_counter_slow_loop = 0; // Slow Loop counter
uint16_t interrupt_counter_fast_loop = 0; // Fast Loop couner
uint32_t interrupt_counter_timer = 0;	  // Timer Counter

void VADC0_G0_2_IRQHandler(void)
{
	interrupt_counter_slow_loop++;
	interrupt_counter_fast_loop++;
	interrupt_counter_timer++;
	/*
	 * Fast compute "loop"
	 * Every 100us compute speed,mechanical/electrical position, field, dq_abc transformation
	 */
	if (TIME_100_us == interrupt_counter_fast_loop)
	{

		// Read and compute into [SI] all currents
		read_currents();
		compute_currents();
		compute_u_mot();
		compute_u_log();

		// Compute all data for motor movement in Operation Enable
		if (motion_config)
		{
			compute_fast_speed();
			compute_fast_mechanical_position();
			compute_fast_electrical_position();
			compute_fast_field();
			abc_dq();

			if (prot_status)
			{
				current_protection();
			}

			// Add Fast Loop regulators
			if (loop_control & I_REF_LOOP_MSK)
			{
				pi_regulator_i_d();
				pi_regulator_i_q();
			}

			dq_abc();
			pwm_update(u_a_ref, u_b_ref);

			ProbeScope_Sampling(); // For Micrium Osciloscope
		}

//		if (prot_status)
//		{
//			current_protection();
//		}

		interrupt_counter_fast_loop = 0;
	}
	/*
	 * Slow compute loop
	 * Every 1ms compute speed
	 */
	if (TIME_1_ms == interrupt_counter_slow_loop)
	{
		if (motion_config)
		{
			compute_speed(); // Compute motor speed and position

			// Reference Generator function and timer
			if (STATUS_0_DISABLED != ref_gen_status)
			{
				interrupt_counter_ref_gen++;
				reference_generator();
			}

			// Slow loop regulators
			if (loop_control & POS_REF_LOOP_MSK)
			{
				pid_regulator_pos();
			}
			if (loop_control & SPD_REF_LOOP_MSK)
			{
				pi_regulator_speed();
			}
		}

		interrupt_counter_slow_loop = 0;
	}
}

/*
 * Interrupt with low priority every 0.2ms
 */
void CCU81_0_IRQHandler(void)
{
	// interrupt_counter_timer++;
}

