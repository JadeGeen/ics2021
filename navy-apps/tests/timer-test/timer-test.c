#include <sys/time.h>
#include <NDL.h>

int main(){
	int t = -1;
	int temp = -1;
	printf("time-test starts\n");
	while(1)
	{
		t = NDL_GetTicks();
		if(t / 500 != temp){
			temp = t/500;
			printf("print every 0.5s : %dth time\n",temp);
		}
	}
	return 0;
}
