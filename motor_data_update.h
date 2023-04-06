#include "main.h"
#ifndef MOTOR_DATA_UPDATE_H_
#define MOTOR_DATA_UPDATE_H_

extern void init_position (void);
extern void compute_speed (void);


extern int32_t motor_spd;
extern int32_t	mechanical_position_old;



#endif /*MOTOR_DATA_UPDATE_H*/
