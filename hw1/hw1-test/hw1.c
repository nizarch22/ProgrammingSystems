#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>


void process(char line[])
{
	int bValidCommand = 0;
	char* commands[5] = {"exit","cd","fork","jobs","cat"}; // remove

	char* argv[1024] = (char*)malloc(64*(char*)); // 64 parameters max - remove if no need to keep track of them
	char command[5];

	char* token = strtok(line," ");
	if(token!=NULL)
	{
		strcpy(command,token);
		printf("command: %s\n",command);
		token = strtok(NULL, " ");
	}
	else
	{
		// No command to analyze
		break; // break out of function (and request another input in the main function loop)
	}
	
	// if argv is removed - remove this while loop
	
	// copying all the parameters to argv
	int paramCount =0; 

	while(token!=NULL)
        {
		if(paramCount>63)
		{
			//ERROR - more than 64 parameters!
		}
        
		printf("part: %s\n",token);
		strcpy(argv[paramCount],token); 

        	token = strtok(NULL, " ");
		paramCount++;
        }
        
	// command processing
	//check if command is non-existent
	
	for(int i=0;i<5;i++)
	{
		if (commands[i]==command)
		{
			bValidCommand = 1;
			break;
		}
	}

	if(bValidCommand)
	{
		//execv(line,argv); // execute the bash command *change cd to chdir*, and what about cat? exit? 
		// does exit automatically wait for forks to be done? Do we need to do it ourselves?
	}
	else
	{
		// ERROR - Invalid command!
	}
	
	free(argv);

}

int main()
{
	char input[1024];
	
	while(1)
	{
		printf("hw1shell$ ");
		gets(input);
		process(input);
	}

}





