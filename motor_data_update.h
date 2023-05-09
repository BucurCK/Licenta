#ifndef MOTOR_DATA_UPDATE_H_
#define MOTOR_DATA_UPDATE_H_

#include "main.h"

extern void init_position (void);
extern void compute_speed (void);


extern int32_t motor_spd;
extern uint16_t timer_value_old_slow;

#define MAX_UINT_16		(65535U)


#endif /*MOTOR_DATA_UPDATE_H*/
