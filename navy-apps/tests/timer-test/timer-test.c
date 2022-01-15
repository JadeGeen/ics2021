#include <sys/time.h>
#include <NDL.h>

int main(){
	float t = -1;
	float temp = -1;
	printf("time-test starts\n");
	while(1)
	{
		t = NDL_GetTicks();
		if(t / 2000 != temp){
			temp = t/1000;
			printf("sec : %f\n",temp);
		}
	}
	return 0;
}
