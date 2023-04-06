// /*
//  * io.c
//  *
//  *  Created on: 8 Dec 2022
//  *      Author: xraid
//  */
// #include "io.h"
// #include "main.h"
// #include "timer.h"

// int status_buton_1;
// int status_buton_2;
// int status_led_1;
// int status_led_2;
// int status_blink_1 = 0;
// int status_blink_2 = 0;
// int status_buton_1_old;
// int status_buton_2_old;
// uint32_t max_value_f = 10000;
// uint32_t wait_1_s_value = 10000;
// uint32_t value_f = 0;
// uint32_t wait_1_s = 0;
// int cond = 0;

// //PWM STUFF START
// int timer_mode = 0;

// void init_io (void)
// {
// 	PORT15->PDISC &= ~DIGITAL_I_12_MSK; 					//enable digital input 12
// 	PORT15->PDISC &= ~DIGITAL_I_13_MSK; 					//enable digital input 13
// 	PORT15->IOCR12 &= ~NO_INT_PUL_12_MSK; 					// set input 12
// 	PORT15->IOCR12 &= ~NO_INT_PUL_13_MSK; 					// set input 13

// 	PORT5->IOCR8 &= ~GENERAL_PURPOSE_OUT_9_MSK; 			// set Output P5.9
// 	PORT5->IOCR8 |= GENERAL_PURPOSE_OUT_ENABLE_9_MSK;
// 	PORT5->IOCR8 |= GPIO_OUTPUT_PUSH_PULL << PORT5_IOCR8_PC9_Pos;
// 	PORT5->IOCR8 &= ~GENERAL_PURPOSE_OUT_8_MSK; 			// set output P5.8
// 	PORT5->IOCR8 |= GENERAL_PURPOSE_OUT_ENABLE_8_MSK;
// 	PORT5->IOCR8 |= GPIO_OUTPUT_PUSH_PULL << PORT5_IOCR8_PC8_Pos;

// //	PORT1->IOCR0 |= (GPIO_OUTPUT_PUSH_PULL << PORT1_IOCR0_PC2_Pos);	//P1.2 as output for LED1_ETHERCAT
// //	PORT5->IOCR0 |= (GPIO_OUTPUT_PUSH_PULL << PORT5_IOCR0_PC3_Pos);	//P5.3 as output for LED1_ETHERCAT

// 	status_buton_1_old = BUTTON_1_MODE;
// 	status_buton_2_old = BUTTON_2_MODE;

// 	status_buton_1 = BUTTON_1_MODE;
// 	status_buton_2 = BUTTON_2_MODE;

// }

// void read_input_1 (void)
// {
// 	status_buton_1 = BUTTON_1_MODE;
// }

// void read_input_2 (void)
// {
// 	status_buton_2 = BUTTON_2_MODE;
// }

// void set_output_blink_1 (void)
// {
// //	PORT1->OMR |= LED_5_ETHERCAT_TOGGLE_OMR;
// 	PORT5->OMR |= LED_1_TOGGLE;

// }

// void set_output_blink_2 (void)
// {
// //	PORT5->OMR |= LED_7_ETHERCAT_TOGGLE_OMR;
// 	PORT5->OMR |= LED_2_TOGGLE;

// }

// void set_output_on_1 (void)
// {
// //	PORT1->OMR |= LED_5_ETHERCAT_ON_OMR;
// 	PORT5->OMR |= LED_1_ON;
// }

// void set_output_on_2 (void)
// {
// //	PORT5->OMR |= LED_7_ETHERCAT_ON_OMR;
// 	PORT5->OMR |= LED_2_ON;
// }

// //void read_output_1 (void)
// //{
// //	status_led_1 = LED_1_MODE;
// //}
// //
// //void read_output_2 (void)
// //{
// //	status_led_2 = LED_2_MODE;
// //}

// void set_OFF (void)
// {
// 	PORT1->OMR |= LED_5_ETHERCAT_OFF_OMR;
// 	PORT5->OMR |= LED_7_ETHERCAT_OFF_OMR;
// }


