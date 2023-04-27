#include "main.h"
#ifndef KERNEL_INTERRUPT_H
#define KERNEL_INTERRUPT_H

extern uint16_t interrupt_counter_timer;

#define TIME_100_us     		(2U)
#define TIME_1_ms       		(20U)
#define LOOP_I_ENABLE			(drive_command_old & 0x100U)
#define LOOP_SPD_ENABLE			(drive_command_old & 0x200U)
#define LOOP_POS_ENABLE			(drive_command_old & 0x400U)

#endif
