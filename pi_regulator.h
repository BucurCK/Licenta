

/*you header file can have declarations here*/


#include <stdint.h>
#include <stdbool.h>
#include <xmc_common.h>
#include "driver_pwm.h"
#include "motor_data_update.h"
#include "main.h"
#include <math.h> //TODO

extern float i_q_ref, i_d_ref, pos_ref, spd_ref;

void pi_regulator_i_d(void);
void pi_regulator_i_q(void);
void pi_regulator_speed(void);
void pi_regulator_pos(void);

