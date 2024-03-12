#include "main.h"   // for HAL GPIO
#include "button.h"

extern unsigned char previous_button_status[BUTTON_NUMBER];

// 1. GPIO 2. GPIO PIN 3. 이전의 버튼

extern int get_button( GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t button_number);

// 1분 : 60sec : 1,000,000us x 60 = 60,000,000us
// 1초 : 1000ms ==> 1ms(1000us) * 1000ms  ==> 1,000,000us
// rpm : revolutions per minutes
// 60,000,000 / 4096 / speed (1~13)
#define STEPS_PER_REV 4096
#define IDLE     0
#define FORWARD  1
#define BACKWARD 2
int  motor_state=0;
void set_rpm(int rpm)   // rpm : 1~13
{
	delay_us(60000000/STEPS_PER_REV/rpm);
	// 최대 speed 기준(13) : delay_us(1126)
}

#if 1
void stepmotor_drive(int direction)  // direction: forward or backward
{
	static int step=0;   // 전역 변수 처럼 동작

	switch(step)
	{
		case 0:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
			break;
		case 1:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
			break;
		case 2:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
			break;
		case 3:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
			break;
		case 4:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
			break;
		case 5:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
			break;
		case 6:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
			break;
		case 7:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
			break;
	}
	if (direction == FORWARD)  // for (i=0; i < 8; i++)문 분리
	{
		step++;
		step %=8;  // 다음 실행 할 step 번호 지정
		// if (step >= 8)
		//    step=0;
	}
	else if (direction == BACKWARD)  // for (i=7; i >= 0; i--)
	{
		step--;
		if (step < 0)
			step=7;
	}
}
#else  // orginal
void stepmotor_drive(int step)
{
	switch(step)
	{
		case 0:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
			break;
		case 1:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
			break;
		case 2:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
			break;
		case 3:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
			break;
		case 4:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
			break;
		case 5:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
			break;
		case 6:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
			break;
		case 7:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
			break;
	}
}
#endif
// btn1: forward / backward
// btn2 or btn3 : stop <--> forward
//                stop <--> backward
void stepmotor_forward_backward_stop()
{
	switch(motor_state)
	{
		case IDLE:
			if (get_button(BUTTON0_GPIO_Port, BUTTON0_Pin, BUTTON0) == BUTTON_PRESS)
			{
				motor_state=FORWARD;
			}
			break;
		case FORWARD:
			stepmotor_drive(FORWARD);
			set_rpm(13);    // rpm: 13 (최대 speed)
			if (get_button(BUTTON1_GPIO_Port, BUTTON1_Pin, BUTTON1) == BUTTON_PRESS)
			{
				motor_state=BACKWARD;
			}
			btn2_btn3_check();
			break;
		case BACKWARD:
			stepmotor_drive(BACKWARD);
			set_rpm(6);    // rpm: 13 (최대 speed)
			if (get_button(BUTTON2_GPIO_Port, BUTTON2_Pin, BUTTON2) == BUTTON_PRESS)
			{
				motor_state=FORWARD;
			}
			btn2_btn3_check();
			break;
	}
}

void btn2_btn3_check()
{
	if (get_button(BUTTON1_GPIO_Port, BUTTON1_Pin, BUTTON1) == BUTTON_PRESS)
	{
		motor_state=IDLE;
	}
	if (get_button(BUTTON2_GPIO_Port, BUTTON2_Pin, BUTTON2) == BUTTON_PRESS)
	{
		motor_state=IDLE;
	}
}
// 시계방향 <---> 반시계 방향
// 1바퀴 회전 하는데 필요한 총 step : 4096
// 4096 / 8(0.7도) == 512 sequence : 360도
// 모터가 360도 회전 하는데 512개의 sequence를 진행 해야 한다.
// 1 sequence 당 : 0.70312도
// 0.70312 x 512 = 360도
// 이걸 이용 해서 동작
// called by main()
void stepmotor_forward_backward()
{
	switch(motor_state)
	{
		case IDLE:
			if (get_button(BUTTON0_GPIO_Port, BUTTON0_Pin, BUTTON0) == BUTTON_PRESS)
			{
				motor_state=FORWARD;
			}
			break;
		case FORWARD:
			stepmotor_drive(FORWARD);
			set_rpm(13);    // rpm: 13 (최대 speed)
			if (get_button(BUTTON0_GPIO_Port, BUTTON0_Pin, BUTTON0) == BUTTON_PRESS)
			{
				motor_state=BACKWARD;
			}
			break;
		case BACKWARD:
			stepmotor_drive(BACKWARD);
			set_rpm(13);    // rpm: 13 (최대 speed)
			if (get_button(BUTTON0_GPIO_Port, BUTTON0_Pin, BUTTON0) == BUTTON_PRESS)
			{
				motor_state=FORWARD;
			}
			break;
	}
}
// 시계방향 <---> 반시계 방향
// 1바퀴 회전 하는데 필요한 총 step : 4096
// 4096 / 8(0.7도)  == 512 sequence : 360도
// 모터가 360도 회전 하는데 512개의 sequence를 진행 해야 한다.
// 1 sequence 당 : 0.70312도
// 0.70312 x 512 = 360도
// 이걸 이용 해서 동작
void stepmotor_main_test()
{
	while (1)
	{
		for (int i=0; i < 512; i++)  // 시계 방향 회전
		{
			for (int j=0; j < 8; j++)  // 1 sequence: 0.7도
			{
				stepmotor_drive(j);
				set_rpm(13);    // rpm: 13 (최대 speed)
			}
		}
		for (int i=0; i < 512; i++)  // 반시계 방향 회전
		{
			for (int j=7; j >= 0; j--)
			{
				stepmotor_drive(j);
				// rpm 만큼 wait
				set_rpm(6);  // half speed
			}
		}
	}
}










