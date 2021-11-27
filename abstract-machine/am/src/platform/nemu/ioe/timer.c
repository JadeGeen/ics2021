#include <am.h>
#include <nemu.h>

void __am_timer_init() {
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
	uptime->us = 0;
	uint32_t uptime_temp1=inl(RTC_ADDR);
	uint64_t uptime_temp2=inl(RTC_ADDR+4);
	uptime->us = (uptime_temp2<<32)+uptime_temp1;
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}
