#ifndef TRANSFORM_H_
#define TRANSFORM_H_

extern void abc_dq(void);
extern void dq_abc(void);
extern void compute_fast_speed (void);
extern void compute_fast_mechanical_position (void);
extern void compute_fast_electrical_position (void);
extern void compute_fast_field (void);
extern void compute_rpm (void);

extern int32_t mechanical_position_fast;
extern int32_t electrical_position_modulo_fast;
extern float theta_fast;
extern float sin_theta_fast;
extern float cos_theta_fast;
extern int16_t timer_value_fast;
extern int16_t timer_value_old_fast;
extern int16_t speed_value_fast;
extern float u_a_ref,u_b_ref,u_c_ref;
extern float u_d,u_q,i_d,i_q;
extern int32_t rpm;
extern float i_alpha, i_beta;


extern void abc_dq_test(void);

#define MAX_UINT_16		(32767)
#define MIN_UINT_16		(-32768)
#define F_1_SQRT_3		(0.5773502f)

#endif /* TRANSFORM_H_ */

