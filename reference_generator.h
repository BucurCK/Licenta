#ifndef REFERENCE_GENERATOR_H_
#define REFERENCE_GENERATOR_H_

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
extern uint32_t interrupt_counter_ref_gen;
extern status_ref_gen ref_gen_status;

void reference_generator (void);
void reference_generator_compute (void);

#define ROTATION_IU			(2000U)			//4 * 500 IU per rotation
#define SECOND				(1000U)			//Slow loop = 1 ms
#define MINUTE				(60U * SECOND)

#endif /* REFERENCE_GENERATOR_H_ */
