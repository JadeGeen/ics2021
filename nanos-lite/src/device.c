#include <common.h>

#if defined(MULTIPROGRAM) && !defined(TIME_SHARING)
# define MULTIPROGRAM_YIELD() yield()
#else
# define MULTIPROGRAM_YIELD()
#endif

#define NAME(key) \
	[AM_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
	[AM_KEY_NONE] = "NONE",
	AM_KEYS(NAME)
};

size_t serial_write(const void *buf, size_t offset, size_t len) {
	size_t i = 0;
	char *buf1 = (char*)buf;
	for(;i<len;i++){
		putch(*buf1);
		buf1++;
	}
	return i;
}

size_t events_read(void *buf, size_t offset, size_t len) {
	AM_INPUT_KEYBRD_T key = io_read(AM_INPUT_KEYBRD);
	if(key.keycode == AM_KEY_NONE)
		return 0;
	else{
		memset(buf, 0, len);
		if(key.keydown)
			sprintf((char*)buf, "kd %s\n", keyname[key.keycode]);
		else
			sprintf((char*)buf, "ku %s\n", keyname[key.keycode]);
		return strlen((char*)buf);
	}
}

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
	uint32_t w = io_read(AM_GPU_CONFIG).width;
	uint32_t h = io_read(AM_GPU_CONFIG).height;
	return sprintf((char*)buf,"WIDTH : %d\nHEIGHT:%d\n",w,h);
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
	uint32_t w = io_read(AM_GPU_CONFIG).width;
	uint32_t h = io_read(AM_GPU_CONFIG).height;
  	uint32_t x = (offset/4)%w;//x
  	uint32_t y = (offset/4)/w;//y
  	if(offset+len > w*h*4) len = w*h*4 - offset;
  	io_write(AM_GPU_FBDRAW, x, y, (uint32_t*)buf, len/4, 1, true);
  	return len;
}

int sys_gettimeofday(struct timeval* tv, struct timezone* tz){
	tv->tv_usec = io_read(AM_TIMER_UPTIME).us;
	tv->tv_sec = tv->tv_usec/1000000;
	return 0;
}

void init_device() {
	Log("Initializing devices...");
	ioe_init();
}
