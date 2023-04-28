#include "main.h"
#ifndef TIMER_H_
#define TIMER_H_

extern void delay_t(uint32_t time);
extern void init_timer(void);
extern void delay_ms(uint32_t time);

#define SECOND_1		(10000U)

#define CCU8_CC8_PRS_PR_MS_VALUE		(28800UL)	//5KHZ = 0.2ms




#endif /* TIMER_H_ */

