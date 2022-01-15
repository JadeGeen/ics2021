#include <sys/time.h>
#include <NDL.h>

int main(){
	int t = 0;
	int temp = -1;
	printf("time-test starts\n");
	while(1)
	{
		t = NDL_GetTicks();
		if(t / 1000 != temp){
			temp = t/1000;
			printf("millisec : %d\n",t);
		}
	}
	return 0;
}
