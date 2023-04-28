#ifndef CURRENT_PROTECTION_H_
#define CURRENT_PROTECTION_H_

#include <math.h>

#define DRIVE_STATUS_PROT_MSK	(0x8000U)
#define TIME_TRIGGER_FAIL		(1000U)					//1[s] = 10000[I.U] --> 0.1s = 1000IU
#define CLEAR_DRIVE_STATUS      ~(0xFU)

extern void current_filter (float_t filter_input);
extern void current_protection (void);


#endif /* CURRENT_PROTECTION_H_ */
