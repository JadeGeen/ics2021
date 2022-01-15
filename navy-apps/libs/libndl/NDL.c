#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

static int evtdev = -1;
static int fbdev = -1;
static int screen_w = 0, screen_h = 0;
static int canvas_w = 0, canvas_h = 0;
static int offset_w = 0, offset_h = 0;
static uint32_t* canvas =NULL;

uint32_t NDL_GetTicks() {
	struct timeval cur_time;
	gettimeofday(&cur_time, NULL);
	return cur_time.tv_usec/1000;
}

int NDL_PollEvent(char *buf, int len) {
	FILE* fp = fopen("/dev/events","r");
	memset(buf, 0, len);
	int ret = fread(buf, 1, len, fp);
	fclose(fp);
	if(ret == 0)return 0;
	for(int i = 0;i<len;i++)
		if(buf[i]=='\n'){
			buf[i]='\0';
			break;
		}
	return ret;
}

void NDL_OpenCanvas(int *w, int *h) {
	FILE* fp = fopen("/proc/dispinfo", "r");
	fscanf(fp, "WIDTH : %d\nHEIGHT:%d", &screen_w, &screen_h);
	if(*w == 0&&*h == 0){
    		canvas_w = screen_w;
    		canvas_h = screen_h;
    		*w = screen_w;
    		*h = screen_h;
  	}
  	else {
		canvas_w = *w<=screen_w? *w:screen_w;		
		canvas_h = *h<=screen_h? *h:screen_h;
	}
	canvas = (uint32_t*)malloc(sizeof(uint32_t)*(*w)*(*h));
	memset(canvas,0,sizeof(canvas));
	offset_w = (screen_w - canvas_w)/2;
	offset_h = (screen_h - canvas_h)/2;//居中
	
	//?
	if (getenv("NWM_APP")) {
		int fbctl = 4;
		fbdev = 5;
		screen_w = *w; screen_h = *h;
		char buf[64];
		int len = sprintf(buf, "%d %d", screen_w, screen_h);
		// let NWM resize the window and create the frame buffer
		write(fbctl, buf, len);
		while (1) {
			// 3 = evtdev
			int nread = read(3, buf, sizeof(buf) - 1);
			if (nread <= 0) continue;
			buf[nread] = '\0';
			if (strcmp(buf, "mmap ok") == 0) break;
		}
		close(fbctl);
	}
}

void NDL_DrawRect(uint32_t *pixels, int x, int y, int w, int h) {
	if(h == 0||h > canvas_h)h = canvas_h;
  	if(w == 0||w > canvas_w)w = canvas_w;
  	for(int i = 0;i < h;i ++)
    		for(int j = 0;j < w;j ++)
      			canvas[(y+i)*canvas_w+(x+j)] = pixels[i*w+j];//canvas x,y处的图像
      	FILE* fp = fopen("/dev/fb","w");
      	for(int i = 0;i < canvas_h;i ++){
    		fseek(fp, 4*((i+offset_h)*screen_w+offset_w), SEEK_SET);
    		fwrite((void*)(canvas+i*canvas_w), 1, 4*canvas_w, fp);//按行输出到屏幕
    	}
}

void NDL_OpenAudio(int freq, int channels, int samples) {
}

void NDL_CloseAudio() {
}

int NDL_PlayAudio(void *buf, int len) {
	return 0;
}

int NDL_QueryAudio() {
	return 0;
}

int NDL_Init(uint32_t flags) {
	if (getenv("NWM_APP")) {
		evtdev = 3;
	}
	return 0;
}

void NDL_Quit() {
}
