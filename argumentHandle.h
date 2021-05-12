#include "builtin.h"

void initShell(char *args[]);

void destructArgs(char *args[]);

void constructArgs(char *args[], int nArgs);

int getNArgs(char *args[]);

int countArgsStr(char *buf);

void strToArgs(char *str, char *args[]);

int getLastApperance(char buf[], char chr);

void getLastHist(char s[], char path[]);

void input(char *args[], char path[]);

int getCharIndex(char *args[], char chr);

int isPipe(char *args[]);

void pipeExec(char *args[]);