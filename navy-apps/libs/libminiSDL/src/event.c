#include <NDL.h>
#include <SDL.h>
#include <assert.h>

#define keyname(k) #k,

static const char *keyname[] = {
	"NONE",
	_KEYS(keyname)
};

int SDL_PushEvent(SDL_Event *ev) {
	printf("please implement me\n");
	assert(0);
	return 0;
}

int SDL_PollEvent(SDL_Event *ev) {
	char buf[1000]={0};
	int ret = NDL_PollEvent(buf, 1000);
	if(ret == 0){ 
		ev->key.keysym.sym = SDLK_NONE;
    		ev->type = SDL_USEREVENT;
		return 0;
	}
	if(strncmp(buf,"kd",2)==0)
		ev->key.type = SDL_KEYDOWN;
	else
		ev->key.type = SDL_KEYUP;
	char *str = buf+3;
	for (int i = 0; i < 83; i++)
      		if (strcmp(keyname[i], str) == 0) {
        		ev->key.keysym.sym = i;
        		return 1;
      		}
      	printf("should not reach here by pollevent\n");
      	assert(0);
	return 0;
}

int SDL_WaitEvent(SDL_Event *event) {
	while (1)
    		if (SDL_PollEvent(event)==1) 
      			break;
	return 1;
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask) {
	printf("please implement me\n");
	assert(0);
	return 0;
}

uint8_t* SDL_GetKeyState(int *numkeys) {
	return NULL;
}
