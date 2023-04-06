#include <stdint.h>
#include <stdbool.h>
#include <xmc_common.h>
#include "driver_pwm.h"
#include "motor_data_update.h"
#include "main.h"
#include <math.h>

typedef enum
{
	STATUS_0_DISABLED,
	STATUS_1_RISE,
	STATUS_2_HIGH_LEVEL,
	STATUS_3_FALL,
	STATUS_4_LOW_LEVEL
}status_ref_gen;

typedef enum
{
	REF_POS,
	REF_SPD,
	REF_I,
	REF_U,

}ref_type;

extern ref_type ref_type_select;

extern int32_t high_level;
extern int32_t high_level_time;
extern int32_t rise_time;
extern int32_t rise_steps;
extern int32_t rise_increment;
extern int32_t fall_time;
extern int32_t fall_steps;
extern int32_t fall_decrement;
extern int32_t low_level;
extern int32_t low_level_time;

extern int32_t rise_delay;
extern int32_t fall_delay;



extern status_ref_gen ref_gen_status;



void reference_generator (void);
void reference_generator_compute (void);



//TODO

