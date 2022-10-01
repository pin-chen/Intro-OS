#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

static int count_fork = 0;

void forkerror(){
	perror("Fork Error!!!");
	exit(1);
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
	count_fork++;
	pid = fork(); //fork1
	if(pid < 0){
		forkerror();	
	}else if(pid == 0){
		output(1);
		count_fork++;
		pid = fork(); //fork2
		if(pid < 0){
                	forkerror();
        	}else if(pid == 0){
			output(2);
			count_fork++;
			pid = fork(); //fork3
			if(pid < 0){
                		forkerror();
       			}else if(pid == 0){
				output(3);
				exit(0);
        		}
			wait(NULL);
			exit(0);
       		}
		wait(NULL);
		exit(0);
	}else{
		wait(NULL);
		count_fork++;
                pid = fork(); //fork4
                if(pid < 0){
                        forkerror();
                }else if(pid == 0){
			output(4);
			count_fork++;
                        pid = fork(); //fork5
                        if(pid < 0){
                                forkerror();
                        }else if(pid == 0){
				output(5);
                                exit(0);
                        }
                        wait(NULL);
                        exit(0);
                }else{
			wait(NULL);
			count_fork++;
                	pid = fork(); //fork6
                	if(pid < 0){
                        	forkerror();
                	}else if(pid == 0){
				output(6);
				count_fork++;
                        	pid = fork(); //fork5
                        	if(pid < 0){
                                	forkerror();
                        	}else if(pid == 0){
					output(5);
                                	exit(0);
                       		}
                        	wait(NULL);
                	        exit(0);
        	        }else{
				wait(NULL);
				count_fork++; 
				pid = fork(); //fork5
				if(pid < 0){
					forkerror();
				}else if(pid == 0){	
					output(5);
					exit(0);
				}
                	}
		}
	}
	return 0;

}
