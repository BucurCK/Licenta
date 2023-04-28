#include <stdint.h>
#include <stdbool.h>
#include "pi_regulator.h"
#include "transform.h"
#include "driver_adc.h"
#include "state_machine.h"

regulator position[2]; // to modify
regulator current_q;
regulator current_d;
regulator speed;
float_t err_old_pos;

float_t i_q_ref = 0, i_d_ref = 0, pos_ref = 0, spd_ref = 0, u_q_ref = 0, u_d_ref;

void pi_regulator_i_q(void) // i_q -> u_q_ref
{
	current_q.error = i_q_ref - i_q;

	// Proportional part computation
	current_q.p_part = current_q.error * current_q.kp;
	if (current_q.p_part > current_q.sat_out)
	{
		current_q.p_part = current_q.sat_out;
	}
	else if (current_q.p_part < -current_q.sat_out)
	{
		current_q.p_part = -current_q.sat_out;
	}

	// Integral part computation
	current_q.i_part += current_q.error * current_q.ki;
	if (current_q.i_part > current_q.sat_i_part)
	{
		current_q.i_part = current_q.sat_i_part;
	}
	else if (current_q.i_part < -current_q.sat_i_part)
	{
		current_q.i_part = -current_q.sat_i_part;
	}

	// Output
	u_q_ref = current_q.p_part + current_q.i_part;

	if (u_q_ref > current_q.sat_out)
	{
		u_q_ref = current_q.sat_out;
	}
	else if (u_q_ref < -current_q.sat_out)
	{
		u_q_ref = -current_q.sat_out;
	}
}

void pi_regulator_i_d(void) // i_d -> u_d_ref
{
	current_d.error = i_d_ref - i_d;

	// Proportional part computation
	current_d.p_part = current_d.error * current_d.kp;
	if (current_d.p_part > current_d.sat_out)
	{
		current_d.p_part = current_d.sat_out;
	}
	else if (current_d.p_part < -current_d.sat_out)
	{
		current_d.p_part = -current_d.sat_out;
	}

	// Integral part computation
	current_d.i_part += current_d.error * current_d.ki;
	if (current_d.i_part > current_d.sat_i_part)
	{
		current_d.i_part = current_d.sat_i_part;
	}
	else if (current_d.i_part < -current_d.sat_i_part)
	{
		current_d.i_part = -current_d.sat_i_part;
	}

	// Output
	u_d_ref = current_d.p_part + current_d.i_part;

	if (u_d_ref > current_d.sat_out)
	{
		u_d_ref = current_d.sat_out;
	}
	else if (u_d_ref < -current_d.sat_out)
	{
		u_d_ref = -current_d.sat_out;
	}
}

void pi_regulator_speed(void) // motor_spd -> i_q_ref
{
	speed.error = spd_ref - motor_spd;

	// Proportional part computation
	speed.p_part = speed.error * speed.kp;
	if (speed.p_part > speed.sat_out)
	{
		speed.p_part = speed.sat_out;
	}
	else if (speed.p_part < -speed.sat_out)
	{
		speed.p_part = -speed.sat_out;
	}

	// Integral part computation
	speed.i_part += speed.error * speed.ki;
	if (speed.i_part > speed.sat_i_part)
	{
		speed.i_part = speed.sat_i_part;
	}
	else if (speed.i_part < -speed.sat_i_part)
	{
		speed.i_part = -speed.sat_i_part;
	}

	// Output
	i_q_ref = speed.p_part + speed.i_part;

	if (i_q_ref > speed.sat_out)
	{
		i_q_ref = speed.sat_out;
	}
	else if (i_q_ref < -speed.sat_out)
	{
		i_q_ref = -speed.sat_out;
	}
}

void pid_regulator_pos(void) // mechanical_position_fast -> spd_ref
{
	int i = !(LOOP_SPD_ENABLE);

	position[i].error = pos_ref - mechanical_position_fast;

	// Proportional part computation
	position[i].p_part = position[i].error * position[i].kp;
	if (position[i].p_part > position[i].sat_out)
	{
		position[i].p_part = position[i].sat_out;
	}
	else if (position[i].p_part < -position[i].sat_out)
	{
		position[i].p_part = -position[i].sat_out;
	}

	// Integral part computation
	position[i].i_part += position[i].error * position[i].ki;
	if (position[i].i_part > position[i].sat_i_part)
	{
		position[i].i_part = position[i].sat_i_part;
	}
	else if (position[i].i_part < -position[i].sat_i_part)
	{
		position[i].i_part = -position[i].sat_i_part;
	}

	// Derived part computation	
	position[i].d_part = (position[i].error - err_old_pos) * position[i].kd;
	if (position[i].d_part > position[i].sat_out)
	{
		position[i].d_part = position[i].sat_out;
	}
	else if (position[i].d_part < -position[i].sat_out)
	{
		position[i].d_part = -position[i].sat_out;
	}

	// Output SPD or I
	switch (i)
	{
	case (SPD_REF):
		spd_ref = position[i].p_part + position[i].i_part + position[i].d_part;

		if (spd_ref > position[i].sat_out)
		{
			spd_ref = position[i].sat_out;
		}
		else if (spd_ref < -position[i].sat_out)
		{
			spd_ref = -position[i].sat_out;
		}
		break;

	case (I_REF):
		i_q_ref = position[i].p_part + position[i].i_part + position[i].d_part;

		if (i_q_ref > position[i].sat_out)
		{
			i_q_ref = position[i].sat_out;
		}
		else if (i_q_ref < -position[i].sat_out)
		{
			i_q_ref = -position[i].sat_out;
		}
		break;

	default:
		break;
	}
}

void pi_init(void)
{
	current_q.kp = 10;
	current_q.ki = 0.05;
	current_q.kd = 0;
	current_q.sat_out = 15000;
	current_q.sat_i_part = current_q.sat_out / 10;

	current_d.kp = current_q.kp;
	current_d.ki = current_q.ki;
	current_d.kd = current_q.kd;
	current_d.sat_out = current_q.sat_out;
	current_d.sat_i_part = current_q.sat_i_part;

	speed.kp = 200;
	speed.ki = 5;
	speed.kd = 0;
	speed.sat_out = 5000;
	speed.sat_i_part = speed.sat_out / 10;

	position[0].kp = 0.3;
	position[0].ki = 0.0015;
	position[0].kd = 0;
	position[0].sat_out = 50;
	position[0].sat_i_part = position[0].sat_out / 10;

	position[1].kp = 0.5;
	position[1].ki = 0.025;
	position[1].kd = 25.0;
	position[1].sat_out = 5000;
	position[1].sat_i_part = position[1].sat_out / 10;

}
