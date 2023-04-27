

/*you header file can have declarations here*/


#include <stdint.h>
#include <stdbool.h>
#include <xmc_common.h>
#include "driver_pwm.h"
#include "motor_data_update.h"
#include "main.h"
#include <math.h> //TODO

#define LOOP_I_ENABLE			(drive_command_old & 0x100U)
#define LOOP_SPD_ENABLE			(drive_command_old & 0x200U)
#define LOOP_POS_ENABLE			(drive_command_old & 0x400U)
#define SPD_REF					(0x0U)
#define I_REF					(0x1U)

extern float_t i_q_ref, i_d_ref, pos_ref, spd_ref, u_q_ref, u_d_ref;

typedef struct regulator
{

	float_t kp;
	float_t ki;
	float_t kd;
	float_t error;
	float_t p_part;
	float_t i_part;
	float_t d_part;
	float_t sat_out;
	float_t sat_i_part;

}regulator;


extern void pi_regulator_i_d(void);
extern void pi_regulator_i_q(void);
extern void pi_regulator_speed(void);
extern void pid_regulator_pos(void);
extern void pi_init(void);

