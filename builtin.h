#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <dirent.h>
#include <sys/stat.h>
#include <wait.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/wait.h>

#define MAX_ARGS 10
#define MAX_ARG_LENGTH 50
#define MAX_CMD_LENGTH 500
#define GREEN "\x1B[32m"
#define NORMAL "\x1B[0m"
#define BLUE "\x1B[34m"


void exitShell(char **args, int nArgs);

int myCompare(const void *a, const void *b);

char **sort(char *arr[], int n);

void cdHelper(char *curDir, char *newDir);

void changeDirectory(char **args, int nArgs);

char **ls(int *n);

char *getPwd();
