#include "builtin.h"
#include "argumentHandle.h"

void executeArgs(char *args[]);

bool execBuiltin(char *args[], int nArgs, char path[]);

void writeToFile(char *input[], int x, char *fileName);

char **readFromFile(char *fileName, int *n);

int containRedirectOut(char *args[]);

int containRedirectIn(char *args[]);

void redirectingIn(char *args[]);

void redirectingOut(char *args[]);

void execExit(char *args[], int nArgs);

void execCD(char *args[], int nArgs);

void execPWD(char *args[], int nArgs);

void execSort(char *args[], int nArgs);

void execLS(char *args[], int nArgs);

void execHistory(char *args[], int nArgs, char path[]);
