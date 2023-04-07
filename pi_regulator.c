#include <stdint.h>
#include <stdbool.h>
#include "pi_regulator.h"
#include "transform.h"
#include  "driver_adc.h"

float k_p = 1.2, k_i = 0.0002
;
float error_i_q = 0, error_i_d = 0, error_spd = 0, error_pos = 0;
float i_q_ref = 0, i_d_ref = 0, pos_ref = 0, spd_ref = 0, u_q_ref = 0, u_d_ref;
float p_part_i_q = 0, p_part_i_d = 0, p_part_spd = 0, p_part_pos = 0;
float i_part_i_q = 0, i_part_i_d = 0, i_part_spd = 0, i_part_pos = 0;

int32_t sat_out_currents = 8000;					//-40[A] & 40[A]
int32_t sat_out_spd = 3000;							//-983010[rpm] & 983010[rpm]
int32_t sat_out_pos = 1000;							//-32bit[iu]	+32bit[iu]

//10% from sat_out
int32_t sat_i_part_currents;
int32_t sat_i_part_spd;
int32_t sat_i_part_pos;

void pi_regulator_i_q (void)						//i_q -> u_q_ref
{
	sat_i_part_currents = sat_out_currents/10;
	error_i_q = i_q_ref - i_q;

	//Proportional part computation
	p_part_i_q = error_i_q*k_p;
	if (p_part_i_q > sat_out_currents)
	{
		p_part_i_q = sat_out_currents;
	}
	else if(p_part_i_q < -sat_out_currents)
	{
		p_part_i_q = -sat_out_currents;
	}

	//Integral part computation
	i_part_i_q += error_i_q*k_i;
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
	p_part_i_d = error_i_d*k_p;
	if (p_part_i_d > sat_out_currents)
	{
		p_part_i_d = sat_out_currents;
	}
	else if(p_part_i_d < -sat_out_currents)
	{
		p_part_i_d = -sat_out_currents;
	}

	//Integral part computation
	i_part_i_d += error_i_d*k_i;
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
	p_part_spd = error_spd*k_p;
	if (p_part_spd > sat_out_spd)
	{
		p_part_spd = sat_out_spd;
	}
	else if(p_part_spd < -sat_out_spd)
	{
		p_part_spd = -sat_out_spd;
	}

	//Integral part computation
	i_part_spd += error_spd*k_i;
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
	p_part_pos = error_pos*k_p;
	if (p_part_pos > sat_out_pos)
	{
		p_part_pos = sat_out_pos;
	}
	else if(p_part_pos < -sat_out_pos)
	{
		p_part_pos = -sat_out_pos;
	}

	//Integral part computation
	i_part_pos += error_pos*k_i;
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
