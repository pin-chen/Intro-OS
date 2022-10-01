#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

void output(int num){
	printf("Fork %d. I'm the chilid %jd, my parent is %jd.\n", num, (intmax_t) getpid(), (intmax_t) getppid());
}

int main(){
	printf("Main Process ID : %jd\n", (intmax_t) getpid());
	if(!fork()){ //fork1
		output(1);
		if(!fork()){ //fork2
			output(2);
			if(!fork()){ //fork3
				output(3);
			}else{
				wait(NULL);
       			}
		}else{
			wait(NULL);
		}
	}else{
		wait(NULL);
                if(!fork()){ //fork4
			output(4);
		}else{
			wait(NULL);
               		if(!fork()){ //fork6
				output(6);
			}else{
				wait(NULL);
			}
		}
		if(!fork()){ //fork5
			output(5);
		}else{
			wait(NULL);
		}
	}
	return 0;
}
