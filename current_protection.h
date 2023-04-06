/*
 * current_protection.h
 *
 *  Created on: 6 Apr 2023
 *      Author: xraid
 */

#ifndef CURRENT_PROTECTION_H_
#define CURRENT_PROTECTION_H_

#define DRIVE_STATUS_FAIL_MSK	(0x8000U)
#define TIME_TRIGGER_FAIL		(1000U)					//1[s] = 10000[I.U]

extern uint32_t timer_prot;

extern void current_filter (void);
extern void current_protection (void);


#endif /* CURRENT_PROTECTION_H_ */
