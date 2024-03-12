/*
 * dcmotor.c
 *
 *  Created on: Apr 6, 2023
 *      Author: sikwon
 */


#include "dcmotor.h"
#include "button.h"
#include "i2c_lcd.h"

#include "cmsis_os.h"

void dcmotor_pwm_control(void);
uint16_t CCR_UP_Value = 0;    // PWM control
uint16_t CCR_DOWN_Value = 0;
uint8_t pwmFlag = 0;
uint8_t auto_mode_state=0;

extern TIM_HandleTypeDef htim3;
extern osMutexId_t myMutex01Handle;

void mode_check();
void drive_car_main();
void auto_drive(void);
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

void drive_car_main()
{
    while (1)
    {
		manual_mode_run();       // bluetooth car command  run
		mode_check();            // button1 check

		if (auto_mode_state)
		{
			auto_drive();
		}
    }

}

void mode_check()
{
	if (get_button(BUTTON0_GPIO_Port, BUTTON0_Pin, BUTTON0) == BUTTON_PRESS)
	{
		auto_mode_state = !auto_mode_state;

		if (osMutexWait(myMutex01Handle, 1000) == osOK) {
			move_cursor(0,0);
			if (auto_mode_state)
			{
				lcd_string("AUTO Mode       ");
			}
			else
			{
				lcd_string("Manual Mode    ");
			}
			move_cursor(1,0);
			lcd_string("                ");
			osMutexRelease(myMutex01Handle);
		}
	}
}

// 자율주행 프로그램을 이곳에 programming 한다.
void auto_drive(void)
{

}

extern volatile uint8_t bt_data;   // 2. BT로 부터 1byte의 INT가 들어오면 저장 하는 변수

void manual_mode_run(void)
{
	if (osMutexWait(myMutex01Handle, 1000) == osOK) {
		move_cursor(1,0);
		if (bt_data == 'F')
		{
			lcd_string("Forward    ");
		}
		else if (bt_data == 'B')
		{
			lcd_string("Backward    ");
		}
		else if (bt_data == 'L')
		{
			lcd_string("Turn Left    ");
		}
		else if (bt_data == 'R')
		{
			lcd_string("Turn Right   ");
		}
//		else
//		{
//			lcd_string("                ");
//		}
		osMutexRelease(myMutex01Handle);
	}


	switch(bt_data)
	{
		case 'F':
		forward(100);
		break;
		case 'B':
		backward(100);
		break;
		case 'L':
		turn_left(100);
		break;
		case 'R':
		turn_right(100);
		break;
		case 'S':
		stop();
		break;
		default:
		break;
	}
}

void forward(int speed)
{
	all_foward();

	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, speed);   // left speed

	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, speed);   //  right speed
}

void backward(int speed)
{
	all_backward();

	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, speed);   // left speed

	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, speed);   //  right speed
}

void turn_left(int speed)
{
	all_foward();

	left_speed(speed);
	right_speed(0);   //  PWM 출력 right
}

void turn_right(int speed)
{
	all_foward();

	left_speed(0); //  PWM 출력	  left
	right_speed(speed);    //  PWM 출력 right
}

void stop()
{
	all_stop();

	HAL_GPIO_WritePin(LEFT_WHEEL_IN1_PORT, LEFT_WHEEL_IN1_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LEFT_WHEEL_IN2_PORT, LEFT_WHEEL_IN2_PIN, GPIO_PIN_SET);

	HAL_GPIO_WritePin(RIGHT_WHEEL_IN3_PORT, RIGHT_WHEEL_IN3_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(RIGHT_WHEEL_IN4_PORT, RIGHT_WHEEL_IN4_PIN, GPIO_PIN_SET);

}


void left_speed(uint16_t speed)
{
	if (speed >= 100) speed = 100;
	else if (speed < 0) speed = 0;

	if (speed == 0)
	{
		HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
	}
	else
	{
		HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, speed);
	}

}

void right_speed(uint16_t speed)
{
	if (speed >= 100) speed = 100;
	else if (speed < 0) speed = 0;

	if (speed == 0)
	{
		HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_2);
	}
	else
	{
		HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, speed);
	}
}

void left_forward()
{
	HAL_GPIO_WritePin(LEFT_WHEEL_IN1_PORT, LEFT_WHEEL_IN1_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LEFT_WHEEL_IN2_PORT, LEFT_WHEEL_IN2_PIN, GPIO_PIN_RESET);
}

void right_forward()
{
	HAL_GPIO_WritePin(RIGHT_WHEEL_IN3_PORT, RIGHT_WHEEL_IN3_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(RIGHT_WHEEL_IN4_PORT, RIGHT_WHEEL_IN4_PIN, GPIO_PIN_RESET);
}

void all_foward()
{
	 left_forward();
	 right_forward();
}

void left_backward()
{
	HAL_GPIO_WritePin(LEFT_WHEEL_IN1_PORT, LEFT_WHEEL_IN1_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LEFT_WHEEL_IN2_PORT, LEFT_WHEEL_IN2_PIN, GPIO_PIN_SET);

}

void right_backward()
{
	HAL_GPIO_WritePin(RIGHT_WHEEL_IN3_PORT, RIGHT_WHEEL_IN3_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RIGHT_WHEEL_IN4_PORT, RIGHT_WHEEL_IN4_PIN, GPIO_PIN_SET);
}

void all_backward()
{
	left_backward();
	right_backward();
}

void left_stop()
{
	left_speed(0);
}

void right_stop()
{
	right_speed(0);
}

void all_stop()
{
	 left_stop();
	 right_stop();
}

void dcmotor_pwm_control(void)
{
	 //------------------ start 선풍기 -------------------------------
	  if (get_button(BUTTON0_GPIO_Port, BUTTON0_Pin, BUTTON0) == BUTTON_PRESS)   // speed-up
	  {
			HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
			CCR_UP_Value = __HAL_TIM_GET_COMPARE(&htim3, TIM_CHANNEL_1);
			CCR_UP_Value = CCR_UP_Value + 10;
			if(CCR_UP_Value > 100) CCR_UP_Value = 100;
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, CCR_UP_Value);
	  }
	  if (get_button(BUTTON1_GPIO_Port, BUTTON1_Pin, BUTTON1) == BUTTON_PRESS)   // speed-down
	  {
			HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
			CCR_DOWN_Value = __HAL_TIM_GET_COMPARE(&htim3, TIM_CHANNEL_1);
			CCR_DOWN_Value = CCR_DOWN_Value - 10;
			if(CCR_DOWN_Value < 70) CCR_DOWN_Value = 70;
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, CCR_DOWN_Value);
	  }

	  if (get_button(BUTTON2_GPIO_Port, BUTTON2_Pin, BUTTON2) == BUTTON_PRESS)   // pwm start/ stop
	  {
			HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);
			if(!pwmFlag)
			{
				pwmFlag = 1;
				HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
			}
			else
			{
				pwmFlag = 0;
				HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
			}
	  }
	//---------------------  end FAN ----------------------------
}
