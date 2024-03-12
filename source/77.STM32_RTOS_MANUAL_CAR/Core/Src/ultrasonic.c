#include "main.h"   // GPIO HAL

#define TRIG_PORT  GPIOC
#define TRIG_PIN   GPIO_PIN_5

// 여기는 cubeIDE와 만나지 않는 영역 이다.
// 한글로 주석을 달아도 깨지지 않는다.
int distance;   // 거리 즉정값을 저장 하는 변수
int ic_cpt_flag;   // rising edge/falling edge를 detect 했을때 check하는 flag
// rising edge/falling edge Interrupt 가 발생 되면 이곳으로 들어 온다.
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	static uint8_t is_first_capture=0;

	if (htim->Instance == TIM1)
	{
		if (is_first_capture == 0)   // rising edge detect !!!
		{
			__HAL_TIM_SET_COUNTER(htim, 0);   // clear
			is_first_capture=1;   // rising edge를 만났다는 indicator를 set
		}
		else if (is_first_capture == 1)  // falling edge detect !!!
		{
			is_first_capture=0;
			distance = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);  // 현재 까지 count
			ic_cpt_flag=1;
		}
	}
}

void make_trigger()
{
	HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_RESET);
	delay_us(2);
	HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_SET);
	delay_us(10);
	HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_RESET);
}

extern volatile int TIM11_10ms_ultrasonic_counter;

void ultrasonc_porcessing()
{
	if (TIM11_10ms_ultrasonic_counter >= 100)  // 1sec reached !!!
	{
		TIM11_10ms_ultrasonic_counter=0;
		make_trigger();
		if (ic_cpt_flag)
		{
			ic_cpt_flag=0;
			distance = distance * 0.034 / 2; // cm로 환산 1us가 0.034cm를 이동,
			// 왕복 값을 return 해 주기 때문에 2로 나눈다. 여기서는 편도값만 필요 하기 떄문
			printf("distance : %d\n", distance);
		}
	}
}











