#include <stdint.h>
#include <stdbool.h>
#include "pi_regulator.h"
#include "transform.h"
#include  "driver_adc.h"

float k_p_i= 0.6, k_i_i = 0.01;
float k_p_spd = 400, k_i_spd =100;
float k_p_pos = 0.05, k_i_pos = 0.001;
float k_p_pos_pid = 6.5, k_i_pos_pid = 0.001, k_d_pos_pid = 0.005;
float error_i_q = 0, error_i_d = 0, error_spd = 0, error_pos = 0, error_pos_old = 0;
float i_q_ref = 0, i_d_ref = 0, pos_ref = 0, spd_ref = 0, u_q_ref = 0, u_d_ref;
float p_part_i_q = 0, p_part_i_d = 0, p_part_spd = 0, p_part_pos = 0;
float i_part_i_q = 0, i_part_i_d = 0, i_part_spd = 0, i_part_pos = 0;
float d_part_pos = 0;

int32_t sat_out_currents = 15000;					//-40[A] & 40[A]
int32_t sat_out_spd = 4000;							//-983010[rpm] & 983010[rpm]
int32_t sat_out_pos = 50;							//-32bit[iu]	+32bit[iu]
int32_t sat_out_pos_I = 4000;

//10% from sat_out
int32_t sat_i_part_currents;
int32_t sat_i_part_spd;
int32_t sat_i_part_pos;
int32_t sat_i_part_pos_I;

void pi_regulator_i_q (void)						//i_q -> u_q_ref
{
	sat_i_part_currents = sat_out_currents/10;
	error_i_q = i_q_ref - i_q;

	//Proportional part computation
	p_part_i_q = error_i_q*k_p_i;
	if (p_part_i_q > sat_out_currents)
	{
		p_part_i_q = sat_out_currents;
	}
	else if(p_part_i_q < -sat_out_currents)
	{
		p_part_i_q = -sat_out_currents;
	}

	//Integral part computation
	i_part_i_q += error_i_q*k_i_i;
	if (i_part_i_q > sat_i_part_currents)
	{
		i_part_i_q = sat_i_part_currents;
	}
	else if(i_part_i_q < -sat_i_part_currents)
	{
		i_part_i_q = -sat_i_part_currents;
	}

	//Output
	u_q_ref = p_part_i_q + i_part_i_q;

	if (u_q_ref > sat_out_currents)
	{
		u_q_ref = sat_out_currents;
	}
	else if(u_q_ref < -sat_out_currents)
	{
		u_q_ref = -sat_out_currents;
	}

}

void pi_regulator_i_d (void)					//i_d -> u_d_ref
{
	sat_i_part_currents = sat_out_currents/10;
	error_i_d = i_d_ref - i_d;

	//Proportional part computation
	p_part_i_d = error_i_d*k_p_i;
	if (p_part_i_d > sat_out_currents)
	{
		p_part_i_d = sat_out_currents;
	}
	else if(p_part_i_d < -sat_out_currents)
	{
		p_part_i_d = -sat_out_currents;
	}

	//Integral part computation
	i_part_i_d += error_i_d*k_i_i;
	if (i_part_i_d > sat_i_part_currents)
	{
		i_part_i_d = sat_i_part_currents;
	}
	else if(i_part_i_d < -sat_i_part_currents)
	{
		i_part_i_d = -sat_i_part_currents;
	}

	//Output
	u_d_ref = p_part_i_d + i_part_i_d;

	if (u_d_ref > sat_out_currents)
	{
		u_d_ref = sat_out_currents;
	}
	else if(u_d_ref < -sat_out_currents)
	{
		u_d_ref = -sat_out_currents;
	}

}

void pi_regulator_speed (void)			//motor_spd -> i_q_ref
{
	sat_i_part_spd = sat_out_spd/10;
	error_spd = spd_ref - motor_spd;

	//Proportional part computation
	p_part_spd = error_spd*k_p_spd;
	if (p_part_spd > sat_out_spd)
	{
		p_part_spd = sat_out_spd;
	}
	else if(p_part_spd < -sat_out_spd)
	{
		p_part_spd = -sat_out_spd;
	}

	//Integral part computation
	i_part_spd += error_spd*k_i_spd;
	if (i_part_spd > sat_i_part_spd)
	{
		i_part_spd = sat_i_part_spd;
	}
	else if(i_part_spd < -sat_i_part_spd)
	{
		i_part_spd = -sat_i_part_spd;
	}

	//Output
	i_q_ref = p_part_spd + i_part_spd;

	if (i_q_ref > sat_out_spd)
	{
		i_q_ref = sat_out_spd;
	}
	else if(u_q_ref < -sat_out_spd)
	{
		i_q_ref = -sat_out_spd;
	}

}
void pi_regulator_pos (void)				//mechanical_position_fast -> spd_ref
{
	sat_i_part_pos = sat_out_pos/10;
	error_pos = pos_ref - mechanical_position_fast;

	//Proportional part computation
	p_part_pos = error_pos*k_p_pos;
	if (p_part_pos > sat_out_pos)
	{
		p_part_pos = sat_out_pos;
	}
	else if(p_part_pos < -sat_out_pos)
	{
		p_part_pos = -sat_out_pos;
	}

	//Integral part computation
	i_part_pos += error_pos*k_i_pos;
	if (i_part_pos > sat_i_part_pos)
	{
		i_part_pos = sat_i_part_pos;
	}
	else if(i_part_pos < -sat_i_part_pos)
	{
		i_part_pos = -sat_i_part_pos;
	}

	//Output
	spd_ref = p_part_pos + i_part_pos;

	if (spd_ref > sat_out_pos)
	{
		spd_ref = sat_out_pos;
	}
	else if(spd_ref < -sat_out_pos)
	{
		spd_ref = -sat_out_pos;
	}

}
void pid_regulator_pos (void)				//mechanical_position_fast -> spd_ref
{
	sat_i_part_pos_I = sat_out_pos_I/10;
	error_pos = pos_ref - mechanical_position_fast;

	//Proportional part computation
	p_part_pos = error_pos*k_p_pos_pid;
	if (p_part_pos > sat_out_pos_I)
	{
		p_part_pos = sat_out_pos_I;
	}
	else if(p_part_pos < -sat_out_pos_I)
	{
		p_part_pos = -sat_out_pos_I;
	}

	//Integral part computation
	i_part_pos += error_pos*k_i_pos_pid;
	if (i_part_pos > sat_i_part_pos_I)
	{
		i_part_pos = sat_i_part_pos_I;
	}
	else if(i_part_pos < -sat_i_part_pos_I)
	{
		i_part_pos = -sat_i_part_pos_I;
	}

	d_part_pos = (error_pos - error_pos_old) * k_d_pos_pid;
	error_pos_old = error_pos;
	if (d_part_pos > sat_out_pos_I)
	{
		d_part_pos = sat_out_pos_I;
	}
	else if(d_part_pos < -sat_out_pos_I)
	{
		d_part_pos = -sat_out_pos_I;
	}

	//Output
	i_q_ref = p_part_pos + i_part_pos + d_part_pos;

	if (i_q_ref > sat_out_pos_I)
	{
		i_q_ref = sat_out_pos_I;
	}
	else if(i_q_ref < -sat_out_pos_I)
	{
		i_q_ref = -sat_out_pos_I;
	}

}
