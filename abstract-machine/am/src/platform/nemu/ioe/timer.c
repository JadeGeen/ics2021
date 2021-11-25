#include <am.h>
#include <nemu.h>

void __am_timer_init() {
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
	uint64_t temp1=inl(RTC_ADDR);
	uint32_t temp2=inl(RTC_ADDR+0x0000020);
	uptime->us = (temp1<<32)+temp2;
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}
