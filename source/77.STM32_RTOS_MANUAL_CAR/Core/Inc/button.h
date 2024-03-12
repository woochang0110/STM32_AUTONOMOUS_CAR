#include "main.h"   // for HAL GPIO handling


#define  BUTTON_PRESS       0     // 버튼을 누르면 0 : active LOW
#define  BUTTON_RELEASE     1     // 버튼을 뗀다.  1
#define  BUTTON_NUMBER      4     // 버튼 갯수

#define  BUTTON0			0
#define  BUTTON1			1
#define  BUTTON2			2
#define  BITTON3            3


void button0_ledall_on_off();
int get_button( GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t button_number);
