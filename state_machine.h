#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_

extern uint16_t drive_status;
extern uint16_t drive_command;
extern uint16_t drive_command_old;
extern uint8_t loop_control;
extern uint8_t motion_config;

extern void drive_disabled(void);
extern void drive_on(void);
extern void operation_enabled(void);
extern void compute_motion (void);
extern void state_machine(void);

#define LOOP_CONTROL_ON					(1U)
#define LOOP_CONTROL_OFF				(0U)
#define STATE_0_DRIVE_DISABLED			(0x0U)
#define STATE_1_DRIVE_ON				(0x1U)
#define STATE_2_OPERATION_ENABLED		(0x2U)



#define DRIVE_COMMAND_STATE_MSK			(drive_command & 0xF)
#define DRIVE_COMMAND_OLD_STATE_MSK		(drive_command_old & 0xF)
#define DRIVE_COMMAND_UPDATE_MSK		(drive_command & 0xF0)
#define DRIVE_COMMAND_UPDATE_OLD_MSK	(drive_command_old & 0xF0)
#define UPDATE_MSK						(0x10U)
#define ON								(0x1U)
#define OFF								(0x0U)
#define DRIVE_COMMAND_REF_MSK			(drive_command & 0xF00)
#define REF_POS_MSK						(0x400U)
#define REF_SPD_MSK						(0x200U)
#define REF_I_MSK						(0x100U)
#define DRIVE_COMMAND_TEST_MSK			(drive_command & 0xF000)
#define TUNE_TEST						(0x8000U)
#define DRIVE_STATUS_MSK				(drive_status & 0xFU)
#define DRIVE_STATUS_CLEAR      		~(0xFU)
#define TUNE_TEST_ENABLE				(drive_command & 0x8000)

#define I_REF_LOOP_MSK                  (0x1U)
#define SPD_REF_LOOP_MSK                (0x2U)
#define POS_REF_LOOP_MSK                (0x4U)
#define TUNE_REF_LOOP_MSK               (0x80U)



#endif /* STATE_MACHINE_H_ */
