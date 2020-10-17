#ifndef __SHELL_H__
#define __SHELL_H__

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <fcntl.h>
#include <dirent.h> 
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define INSTRUCTION_WRONG 0
#define INSTRUCTION_CD 1
#define INSTRUCTION_EXIT 2
#define INSTRUCTION_HISTORY 3
#define INSTRUCTION_SYS_EXE 4

#define SYSTEM_EXE 1
#define NOT_SYSTEM_EXE 0

#define REDIRECTION_IN 1
#define REDIRECTION_OUT 2
#define REDIRECTION_OUT_ADD 3
#define REDIRECTION_PIPE 4
#define REDIRECTION_NONE 0

#define STDIN 0
#define STDOUT 1 
#define STDERR 2


char ** getParameter(char* args);
int shell();
void trim(char* args);
int cd(char** args);
char * getUserName();
int systemExe(char** args);
int whichOperation(char**args);
int redirectionType(char** args);
char* getRedirectionFile(char** args, int type);
char* cmdPreprocess(char* cmd);
#endif // 
