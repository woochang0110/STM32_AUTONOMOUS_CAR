/*
 * dcmotor.h
 *
 *  Created on: Apr 6, 2023
 *      Author: sikwon
 */

#ifndef INC_DCMOTOR_H_
#define INC_DCMOTOR_H_

#include "main.h"
/*
  1. LEFT MOTOR
     PC.6 : IN1
	 PC.7 : IN2
  2. RIGHT MOTOR
     PC.8 : IN3
	 PC.9 : IN4

	 IN1.IN3  IN2.IN4
	 ======= ========
	   0        1  : 역회전
	   1        0  : 정회전
	   1        1  : STOP
*/
#define LEFT_WHEEL_IN1_PORT	GPIOC       // MOTOR_LEFT_A_PORT
#define LEFT_WHEEL_IN2_PORT	GPIOC       // MOTOR_LEFT_B_PORT
#define LEFT_WHEEL_IN1_PIN	GPIO_PIN_6   // MOTOR_LEFT_A_PIN
#define LEFT_WHEEL_IN2_PIN	GPIO_PIN_7   // MOTOR_LEFT_B_PIN

#define RIGHT_WHEEL_IN3_PORT GPIOC      // MOTOR_RIGHT_A_PORT
#define RIGHT_WHEEL_IN4_PORT GPIOC      // MOTOR_RIGHT_B_PORT
#define RIGHT_WHEEL_IN3_PIN	GPIO_PIN_8  // MOTOR_RIGHT_A_PIN
#define RIGHT_WHEEL_IN4_PIN	GPIO_PIN_9  // MOTOR_RIGHT_B_PIN

void left_speed(uint16_t speed);
void right_speed(uint16_t speed);
void left_forward();
void right_forward();
void foward();
void left_backward();
void Right_Backward();
void backward();
void left_stop();
void right_stop();
void stop();
void all_stop();
void all_backward();
void all_foward();


#endif /* INC_DCMOTOR_H_ */
