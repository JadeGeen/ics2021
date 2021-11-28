#include <am.h>
#include <nemu.h>

#define SYNC_ADDR (VGACTL_ADDR + 4)

void __am_gpu_init() {
	int i;
	int w = inw(VGACTL_ADDR+2);  // TODO: get the correct width
	int h = inw(VGACTL_ADDR);  // TODO: get the correct height
	uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
	for (i = 0; i < w * h; i ++) fb[i] = i;
	outl(SYNC_ADDR, 1);
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = inw(VGACTL_ADDR+2), .height = inw(VGACTL_ADDR),
    .vmemsz = 0
  };
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
  	int x = ctl->x, y = ctl->y, w = ctl->w, h = ctl->h;
  	if (w == 0 || h == 0) return;
  	int width=(VGACTL_ADDR+2);
  	uint32_t *pixels=(uint32_t *)ctl->pixels;
  	int i=0;
  	for(int y0=y;y0<y+h;y0++)
  		for(int x0=x;x0<x+w;x0+=4,i++)
  			outl(FB_ADDR+y0*width+x0,pixels[i]);
  	if (ctl->sync) {
    		outl(SYNC_ADDR, 1);
  	}
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
