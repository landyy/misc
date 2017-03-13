#define _GNU_SOURCE
/*
*By: Russell Babarsky
*
*Just testing some basic hooking and shit
*
*/
#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <stdarg.h>

//walk the plank matey
int printf(const char *format, ...){
	
	typeof(printf) *old_printf;
	va_list list;
	char *parg;

	//format stuff for printf idk
	va_start(list, format);	
	vasprintf(&parg, format, list);
	va_end(list);

	//evil stuff >:)	
	system("echo 'walk the plank!'");

	//
	old_printf = dlsym(RTLD_NEXT, "printf");
	(*old_printf)("%s",parg);

//	free(parg); 

}


