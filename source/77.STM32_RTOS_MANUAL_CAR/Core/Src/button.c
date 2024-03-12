#include "button.h"


unsigned char previous_button_status[BUTTON_NUMBER] =
{BUTTON_RELEASE,BUTTON_RELEASE,BUTTON_RELEASE,BUTTON_RELEASE};


// 1. GPIO 2. GPIO PIN 3. 이전의 버튼

int get_button( GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t button_number)
{
	unsigned char current_state;

	current_state = HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);

	if (current_state == BUTTON_PRESS && previous_button_status[button_number] == BUTTON_RELEASE)  // 처음 누른상태
	{
		HAL_Delay(100);   // noise가 지나가기를 기다린다.
		previous_button_status[button_number] = current_state;
		return BUTTON_RELEASE;    // 아직 버튼이 눌러 지지 않는것으로 처리 0을 리턴
	}
	else if (current_state == BUTTON_RELEASE && previous_button_status[button_number] == BUTTON_PRESS)
	{
		previous_button_status[button_number] = BUTTON_RELEASE;   // 릴리즈 상태
		return BUTTON_PRESS;   // 버튼이 완전히 눌렀다 뗀 상태로 판단 하고 1을 리턴 한다.
	}

	return BUTTON_RELEASE;    // 버튼이 눌렀다 떼어진 상태가 아니다.
}
