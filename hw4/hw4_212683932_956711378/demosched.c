#include "types.h"
#include "user.h"

#define NUM_CHILDREN 32 // gives us a number of samples to be assigned different priorities (uniformly - mulitplicity of 8)
#define TEST_PARAM 35 // determines the computation time of the recursive function

/* This code uses a number of children (NUM_CHILDREN) of a single parent. Each executes a CPU BOUND code from the function 'foo', practically a fibonacci sequence.
 * The children 'send' their priority number, and runtime result (using the function uptime()) to the parent using piping(fds[]). 
 * The parent will then store all the results (using pPriority[] and pRuntime[]). And compute the average runtime of each priority (avg[]).
 *
 * More on the parent inside and outside the loop: the wait() function (used on the parent) will run one child at a time, and once the child finishes and sends its data,
 * the exit() function at the end of the child will send us back to the next command of the parent (PC+1). That is why we have to use the wait() function 32 times
 * which is the number of total children created (NUM_CHILDREN).
 *
 * Prints are only used at the end so as to minimally disrupt the runtimes of the children processes.
 */



// CPU BOUND Function
int foo(int a)
{
	if(a==0||a==1)
		return a;
	return (foo(a-1)+foo(a-2));
}


int main(void)
{
	int f; // forking pid
	int start, end, runtime; // variables for tracking clock ticks of CPU BOUND function
	int priority; // priority variables for the children

	// variables that store children processes' priorities and runtimes, and their average.
	int pPriority[NUM_CHILDREN];
	int pRuntime[NUM_CHILDREN];
	int avg[8]={0};
	
	// piping variable for children to write results to parent
	int fds[2];
	pipe(fds);
	
	for(int i=0;i<NUM_CHILDREN;i++)
	{
		f = fork();
		if(f<0)
		{
			printf(1,"ERROR fork could not create child\n");
			exit();
			return -1;
		}
		if(f!=0){} // parent will do nothing during loop
		if(f==0)
		{	
			// set priority
			priority = i%8;
			setprio(priority);
			
			// calculate runtime of CPU BOUND function 'foo'
			start = uptime();
			foo(TEST_PARAM);
			end = uptime();
			runtime = end -start;
			
			// write results to parent
			write(fds[1],&priority,1);
			write(fds[1],&runtime,1);
			exit();
		}

	}
	
	// running all children and storing their results
	for(int i=0;i<NUM_CHILDREN;i++)
	{
		// let 1 child run
		wait(); 

		// parent receives the results from the children
		read(fds[0],&pPriority[i],1);
		read(fds[0],&pRuntime[i],1);
	}

	// computing the sum of all runtimes according to their priority
	for(int i=0;i<NUM_CHILDREN;i++)
	{
		avg[pPriority[i]] += pRuntime[i];
	}

	// number of samples of each priority
	int num = NUM_CHILDREN/8;
	
	// computing the average of each priority's runtime, and printing it.
	for(int i=0;i<8;i++)
	{
		avg[i]/=num;
		printf(1,"Priority: %d - Runtime(average): %d\n",i,avg[i]);
	}

	exit();
}
