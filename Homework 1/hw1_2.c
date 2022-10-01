#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

void forkerror(){
	printf("Fork Error!!!");
}

void output(int num){
	pid_t my_pid = getpid();
	pid_t parent_pid = getppid();
	printf("Fork %u. I'm the chilid %jd, my parent is %jd.\n", num, my_pid, parent_pid);
}

int main(){
	pid_t pid;
	pid = getpid();
	printf("Main Process ID : %jd\n", (intmax_t) pid);
	pid = fork(); //fork1
	if(pid < 0){
		forkerror();	
	}else if(pid == 0){
		output(1);
		pid = fork(); //fork2
		if(pid < 0){
                	forkerror();
        	}else if(pid == 0){
			output(2);
			pid = fork(); //fork3
			if(pid < 0){
                		forkerror();
       			}else if(pid == 0){
				output(3);
        		}
			wait(NULL);
       		}
		wait(NULL);
	}else{
		wait(NULL);
                pid = fork(); //fork4
                if(pid < 0){
                        forkerror();
                }else if(pid == 0){
			output(4);
                        //link5
                }else{
			wait(NULL);
                	pid = fork(); //fork6
                	if(pid < 0){
                        	forkerror();
                	}else if(pid == 0){
				output(6);
                        	//linkl5
        	        }else{
				wait(NULL); 
				//link5
                	}
		}
		pid = fork(); //fork5
		if(pid < 0){
			forkerror();
		}else if(pid == 0){
			output(5);
		}else{
			wait(NULL);
		}
	}
	return 0;
}
