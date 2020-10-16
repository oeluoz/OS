#ifndef __SHELL_H__
#define __SHELL_H__

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

char ** getParameter(char* args);
int shell();
void trim(char* args);
int cd(char** args);
#endif // 
