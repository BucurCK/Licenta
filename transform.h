#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include <math.h>

extern void abc_dq(void);
extern void dq_abc(void);
extern void compute_fast_speed (void);
extern void compute_fast_mechanical_position (void);
extern void compute_fast_electrical_position (void);
extern void compute_fast_field (void);

extern int32_t mechanical_position_fast;
extern float_t u_a_ref,u_b_ref,u_c_ref;
extern float_t i_d,i_q;
extern float_t i_alpha, i_beta;
extern uint16_t timer_value_old_fast;
extern float_t theta_fast;


extern void abc_dq_test(void);

#define MAX_UINT_16		(32767)
#define MIN_UINT_16		(-32768)
#define F_1_SQRT_3		(0.5773502f)

#endif /* TRANSFORM_H_ */

