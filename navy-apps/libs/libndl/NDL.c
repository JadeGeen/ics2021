#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <assert.h>

static int evtdev = -1;
static int fbdev = -1;
static int screen_w = 0, screen_h = 0, canvas_w = 0, canvas_h = 0;
static uint32_t* canvas;

uint32_t NDL_GetTicks() {
  struct timeval current_time;
  gettimeofday(&current_time, NULL);
  return current_time.tv_usec / 1000;
}

int NDL_PollEvent(char *buf, int len) {
  FILE* evt_file = fopen("/dev/events", "r");
  char ch = -1;
  int i = 0;
  while (i < len && (ch = fgetc(evt_file)) != EOF) {
    buf[i++] = ch;
  }
  fclose(evt_file);
  return i ? 1 : 0;
}

void NDL_OpenCanvas(int *w, int *h) {
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
  } else {
    if (*w == 0 && *h == 0) *w = screen_w, *h = screen_h;
    canvas_w = *w; canvas_h = *h;
    assert(canvas_w <= screen_w && canvas_h <= screen_h);
    canvas = malloc(canvas_w * canvas_h * sizeof(uint32_t));
  }
}

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte >> 24) & 0xff, \
  (byte >> 16) & 0xff, \
  (byte >> 8) & 0xff, \
  (byte) & 0xff

void NDL_UpdateCanvas() {
  FILE* fb_file = fopen("/dev/fb", "w");
  uint32_t tmp0 = 0;
  int h_offset = (screen_h - canvas_h) / 2, 
      w_offset = (screen_w - canvas_w) / 2;
  for (int i = -h_offset; i < screen_h - h_offset; i++)
    for (int j = -w_offset; j < screen_w - w_offset; j++) {
      if (i >= 0 && j >= 0 && i < canvas_h && j < canvas_w) {
        fwrite(&canvas[i * canvas_w + j], sizeof(uint32_t), 1, fb_file);
      } else {
        fwrite(&tmp0, sizeof(uint32_t), 1, fb_file);
      }
    }
  fclose(fb_file);
}

void NDL_DrawRect(uint32_t *pixels, int x, int y, int w, int h) {
  if (x == 0 && y == 0 && w == 0 && h == 0) {
    w = canvas_w, h = canvas_h;
  }
  for (int i = 0; i < h; i++)
    for (int j = 0; j < w; j++)
      canvas[(y + i) * canvas_w + (x + j)] = pixels[(y + i) * canvas_w + (x + j)];

  NDL_UpdateCanvas();
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
  FILE* dispinfo_file = fopen("/proc/dispinfo", "r");
  fscanf(dispinfo_file, "WIDTH : %d\nHEIGHT:%d", &screen_w, &screen_h);
  return 0;
}

void NDL_Quit() {
}
