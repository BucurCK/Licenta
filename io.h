/*
 * io.h
 *
 *  Created on: 8 Dec 2022
 *      Author: xraid
 */
#include "main.h"

#define SET13 	0x2000
#define SET12 	0x1000

#define NO_INT_PULL			0X7
#define GENERAL_PURPOSE_OUT	0xF

#define DIGITAL_I_12_MSK 					(1UL << 12U)
#define DIGITAL_I_13_MSK					(1UL << 13U)
#define NO_INT_PUL_12_MSK					(NO_INT_PULL << 3U)
#define NO_INT_PUL_13_MSK					(NO_INT_PULL << 11U)
#define GENERAL_PURPOSE_OUT_8_MSK			(GENERAL_PURPOSE_OUT << 3U)
#define GENERAL_PURPOSE_OUT_9_MSK			(GENERAL_PURPOSE_OUT << 11U)
#define GENERAL_PURPOSE_OUT_ENABLE_8_MSK	(1UL << 7U)
#define GENERAL_PURPOSE_OUT_ENABLE_9_MSK	(1UL << 15U)

#define LED_1_ON		(1UL << 9U)		//PS - P5.9
#define LED_1_OFF		(1UL << 25U)	//PR - P5.9
#define LED_2_ON		(1UL << 8U)		//PS - P5.8
#define LED_2_OFF		(1UL << 24U)	//PR - P5.8
#define LED_2_TOGGLE	((1UL << 24U) | (1UL << 8U))
#define LED_1_TOGGLE	((1UL << 25U) | (1UL << 9U))

#define BUTTON_1_MODE	((PORT15->IN & (1UL << 13U)) >> 13U)	//PORT15_13_INPUT_MASK
#define BUTTON_2_MODE	((PORT15->IN & (1UL << 12U)) >> 12U)	//PORT15_12_INPUT_MASK

#define LED_1_MODE		((PORT5->OUT & (1UL << 9U)) >> 9U)		//PORT5_9_OUTPUT_MASK
#define LED_2_MODE		((PORT5->OUT & (1UL << 8U)) >> 8U)		//PORT5_8_OUTPUT_MASK

#define LED_5_ETHERCAT_ON_OMR		(1UL << 2UL)
#define LED_5_ETHERCAT_OFF_OMR		(1UL << 18UL)
#define LED_5_ETHERCAT_TOGGLE_OMR	(1UL << 2UL | 1UL << 18UL)

#define LED_7_ETHERCAT_ON_OMR		(1UL << 3UL)
#define LED_7_ETHERCAT_OFF_OMR		(1UL << 19UL)
#define LED_7_ETHERCAT_TOGGLE_OMR	(1UL << 3UL | 1UL << 19UL)

#define PORT0_13_TEST_INPUT ((PORT0->IN & (1UL << 13U)) >> 13U)
#define PORT14_7_TEST_INPUT ((PORT14->IN & (1UL << 7U)) >> 7U)


extern int status_buton_1;
extern int status_buton_2;
extern int status_buton_1_old;
extern int status_buton_2_old;
extern int status_led_1;
extern int status_led_2;
extern int status_blink_1;
extern int status_blink_2;
extern uint32_t max_value_f;
extern uint32_t value_f;
extern uint32_t wait_1_s;
extern uint32_t wait_1_s_value;
extern int cond;
extern int timer_mode;


extern void read_input_1 (void);
extern void read_input_2 (void);

extern void set_output_on_1 (void);
extern void set_output_on_2 (void);
extern void set_output_blink_1 (void);
extern void set_output_blink_2 (void);

extern void init_io (void);

extern void read_output_1 (void);
extern void read_output_2 (void);

extern void set_OFF (void);
