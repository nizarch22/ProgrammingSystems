#include "types.h"
#include "user.h"
#include "processInfo.h"

enum procstate { UNUSED, EMBRYO, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };
char* strStates[] = {"UNUSED", "EMBRYO", "SLEEPING", "RUNNABLE", "RUNNING", "ZOMBIE"};
int main(void)
{
	int num, maxPid;
	struct processInfo pi;
	num = getNumProc();
	maxPid = getMaxPid();
	int test = getProcInfo(maxPid, &pi);
	printf(1,"maxPID - test: %d\n",test);

	printf(1,"Total number of active processes: %d\n",num);
	printf(1,"Maximum PID: %d\n",maxPid);
	printf(1,"PID\tSTATE\tPPID\tSZ\tNFS\tNRSWITCH\n");
	
	int err;
	for(int pid=0;pid<=maxPid;pid++)
	{
		err = getProcInfo(pid,&pi);
		if (err==-1){
			//printf(1,"[Error!] such pid: %d - does not exist!\n",pid);
			continue;
		}
		printf(1,"%d\t%d\t%d\t%d\t\%d\t%d\n",pid,pi.state,pi.ppid,pi.sz,pi.nfd,pi.nrswitch);
		//printf(1,"%d\n%d\n%d\n%d\n\%d\n%d\n------\n",pid,pi.state,pi.ppid,pi.sz,pi.nfd,pi.nrswitch);
	}
	exit();
}
