#include "types.h"
#include "user.h"

enum procstate { UNUSED, EMBRYO, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };
char* strStates[] = {"UNUSED", "EMBRYO", "SLEEPING", "RUNNABLE", "RUNNING", "ZOMBIE"};
int main(void)
{
	int num, maxPid;
	num = getNumProc();
	maxPid = getMaxPid();	

	printf(1,"Total number of active processes: %d\n",num);
	printf(1,"Maximum PID: %d",maxPid);
	printf(1,"PID\tSTATE\tPPID\tSZ\tNFS\tNRSWITCH\n");
	
	for(int i=0;i<num;i++)
	{
		getProcInfo(pid,&procInfo);
		printf(1,"%d\t%s\t%d\t%d\t\%d\t%d\n",pid,strStates[state],ppid,sz,nfd,nrs);
	}
	exit();
}
