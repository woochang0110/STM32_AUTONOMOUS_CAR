#include "main.h"   // for GPIO HAL

extern RTC_HandleTypeDef hrtc;

RTC_TimeTypeDef sTime = {0};   // 시간정보
RTC_DateTypeDef sDate = {0};   // 날짜정보

// 예) 22의 BCD의 메모리에 저장된 format
// high nibble low nibble
//   0010       0010
//     20    +     2
//  ==> 22
// bin to dec
unsigned char bin2dec(unsigned char byte)
{
	unsigned char high, low;

	low = byte & 0x0f;   // 하위 4bit만 취한다.
	high = ((byte >> 4) & 0x0f) * 10;   //  00100010  ---> 4bit 오른쪽 ==> 00000010 * 10 ==> 22

	return high + low;
}
// 10진수 --> BCD                  22
unsigned char dec2bin(unsigned char byte)
{
	unsigned char high, low;

	high = ((byte / 10) << 4);
	//00000010 : (byte / 10)
	//00100000 : (byte / 10) << 4
	low = byte % 10;
	// 00000010 <== 22 % 10

	return high + low;   // 0010 0010
}

// RTC 에서 날짜와 시각 정보를 가져오는 함수
void get_rtc_time(void)
{
	static RTC_TimeTypeDef oTime;   // 이전 시각 정보
	char lcd_buff[40];

	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BCD);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BCD);

	if (oTime.Seconds != sTime.Seconds)
	{
		// YYYY-MM-DD HH:mm:SS
		printf("%04d-%02d-%02d %02d:%02d:%02d\n",
				bin2dec(sDate.Year)+2000, bin2dec(sDate.Month),bin2dec(sDate.Date),
				bin2dec(sTime.Hours), bin2dec(sTime.Minutes), bin2dec(sTime.Seconds));
		sprintf(lcd_buff,"%02d-%02d-%02d %02d:%02d:%02d",
				bin2dec(sDate.Year), bin2dec(sDate.Month),bin2dec(sDate.Date),
				bin2dec(sTime.Hours), bin2dec(sTime.Minutes), bin2dec(sTime.Seconds));
		move_cursor(1,0);
		lcd_string(lcd_buff);
	     // text lcd의 1번째 라인의 0번째 col에 년월일 시분초를 출력
		 // 22-08-30 14:10:00
		 // 0번째 : DATE: 2022-08-30
		 // 1    : TIME: 15:50-55
	}
	oTime.Seconds = sTime.Seconds;
}
// setrtc220830111500
// 220830111500
// 012345678901234567
// date_time이라는 변수에는 220830111500의 주소가 넘어 온다
void set_rtc_date_time(char *date_time)
{
	char yy[4], mm[4], dd[4];  // 날짜
	char hh[4], min[4], ss[4];  // 시각

	strncpy(yy, date_time, 2);
	strncpy(mm, date_time+2, 2);
	strncpy(dd, date_time+4, 2);

	strncpy(hh, date_time+6, 2);
	strncpy(min, date_time+8, 2);
	strncpy(ss, date_time+10, 2);

	// 1.ascii --> int --> bcd --> RTC
	sDate.Year = dec2bin(atoi(yy));
	sDate.Month = dec2bin(atoi(mm));
	sDate.Date = dec2bin(atoi(dd));

	sTime.Hours = dec2bin(atoi(hh));
	sTime.Minutes = dec2bin(atoi(min));
	sTime.Seconds = dec2bin(atoi(ss));

	HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD);
	HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD);
}



















