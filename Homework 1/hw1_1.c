#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

void output(int num){
        printf("Fork %u. I'm the chilid %jd, my parent is %jd.\n", num, (intmax_t) getpid(), (intmax_t) getppid());
}
int main(){
	printf("Main Process ID : %jd\n", (intmax_t) getpid());
	pid_t pid;
	pid = fork(); //fork0;
	if(pid == 0){
		output(0);
		pid = fork(); //fork1
		if(pid > 0)
			wait(NULL);
		else if(pid == 0){
			output(1);
			pid = fork(); //fork2
			if(pid > 0)
				wait(NULL);
			else
				output(2);
		}
	}
	else if(pid > 0){
		wait(NULL);
		pid = fork(); //fork3
		if(pid > 0)
			wait(NULL);
		else
			output(3);
	}
	else{
		printf("Error!");
	}
	pid = fork(); //fork4
	if(pid > 0)
		wait(NULL);
	else
		output(4);
	return 0;
}
