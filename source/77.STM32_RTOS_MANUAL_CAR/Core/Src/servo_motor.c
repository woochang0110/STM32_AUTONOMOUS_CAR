#include "main.h"

extern TIM_HandleTypeDef htim2;
extern volatile int TIM11_10ms_servomotor_counter;
// 84000000HZ / 1680 ==> 50,000HZ
// T=1/f 1/ 50000 --> (1펄스의 소요시간)
// 2ms ==> 0.00002sec x 100 --> 0.002sec(2ms) : arm : 180도 회전
// 1ms ==>  0.00002sec x 50 : arm 0도 회전
// 1.5ms===> 0.00002sec x 75 :  arm 90도 회전
int arm_roates_indicator=0;
void servo_motor_control_main()
{
#if 1
	if (TIM11_10ms_servomotor_counter >= 100)   // 1sec
	{
		TIM11_10ms_servomotor_counter=0;
		if (!arm_roates_indicator)
		{
			  // 180도 회전
			  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 110); // PB.10
		}
		else
		{
			 // 0도
			 __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 10);
		}
		arm_roates_indicator = !arm_roates_indicator;  // next arm indicator
	}
#else  // org
	while(1)
	{
	  // 180도 회전
	  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 100); // PB.10
	  HAL_Delay(1000);   // 1초 유지
	  // 0도
	  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 50);
	  HAL_Delay(1000);   // 1초 유지
	}
#endif
}
