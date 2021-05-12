#include "argumentHandle.h"
void initShell(char *args[])
{
    for (int i = 0; i < MAX_ARGS; i++)
    {
        args[i] = NULL;
    }
}

void destructArgs(char *args[])
{
    int i = 0;
    while (args[i] != NULL)
    {
        free(args[i]);
        args[i] = NULL;
    }
}

void constructArgs(char *args[], int nArgs)
{

    for (int i = 0; i < nArgs; i++)
    {
        args[i] = (char *)malloc(MAX_ARG_LENGTH * sizeof(char));
        for (int j = 0; j < MAX_ARG_LENGTH; j++)
        {
            args[i][j] = '\0';
        }
    }
}

int getNArgs(char *args[])
{
    int i;
    for (i = 0; args[i] != NULL; i++)
    {
    }
    return i;
}

int countArgsStr(char *buf)
{
    int count = 0;
    int length = strlen(buf);
    for (int i = 0; i < length; i++)
    {
        if (i == 0)
        {
            count++;
        }
        else
        {
            if (buf[i] != ' ' && buf[i - 1] == ' ')
            {
                count++;
            }
        }
    }
    return count;
}

void strToArgs(char *str, char *args[])
{
    int number_of_args = countArgsStr(str);
    for (int i = 0; i < number_of_args; i++)
    {

        while (str[0] == ' ')
        {
            str++;
        }
        int j = 0;
        while (str[0] != '\0' && str[0] != '\n' && str[0] != ' ')
        {
            args[i][j] = str[0];
            str++;
            j++;
        }
    }
}

int getLastApperance(char buf[], char chr)
{
    int len = strlen(buf);
    for (int i = len - 1; i >= 0; i--)
    {
        if (buf[i] == chr)
        {
            return i;
        }
    }
    return -1;
}
void getLastHist(char s[], char path[])
{
    FILE *file = fopen(path, "r");
    int i = 0;
    if (file)
    {
        fscanf(file, "%d", &i);
        char ch;
        fscanf(file, "%c", &ch);
        int j=0;
        while (fgets(s, MAX_CMD_LENGTH, file)&&j<i)
        {
            j++;
        }
        printf("%s\n", s);
        fclose(file);
    }
}
void updateHistory(char buf[], char path[])
{
    FILE *file = fopen(path, "r");
    char *s;
    char *list[10];
    int i = 0;
    int cnt = 0;
    if (file)
    {

        fscanf(file, "%d", &cnt);
        char ch;
        fscanf(file, "%c", &ch);
        if (cnt == 10)
        {
            s = calloc(MAX_CMD_LENGTH, sizeof(char));
            fgets(s, MAX_CMD_LENGTH, file);
            free(s);
        }

        s = calloc(MAX_CMD_LENGTH, sizeof(char));
        while (fgets(s, MAX_CMD_LENGTH, file))
        {
            list[i] = s;
            s = calloc(MAX_CMD_LENGTH, sizeof(char));
            i++;
        }
        fclose(file);
    }

    file = fopen(path, "w");
    fprintf(file, "%d\n", i + 1);
    for (int it = 0; it < i; it++)
    {
        fprintf(file, "%s", list[it]);
        free(list[it]);
    }
    fprintf(file, "%s", buf);
    fclose(file);
}

void input(char *args[], char path[])
{
    char str[MAX_CMD_LENGTH];
    fgets(str, MAX_CMD_LENGTH, stdin);
    if (strcmp(str, "!!\n") == 0)
    {
        getLastHist(str, path);
    }
    else
    {
        updateHistory(str, path);
    }
    int nArgs = countArgsStr(str);

    constructArgs(args, nArgs);

    strToArgs(str, args);
}

int getCharIndex(char *args[], char chr)
{
    int nArgs = getNArgs(args);
    for (int i = 0; i < nArgs; i++)
    {
        if (args[i][0] == chr)
        {
            return i;
        }
    }
    return -1;
}

int isPipe(char *args[])
{
    int nArgs = getNArgs(args);
    for (int i = 0; i < nArgs; i++)
    {
        if (args[i][0] == '|')
        {
            return 1;
        }
    }
    return 0;
}
int containRedirectOut(char *args[])
{
    int nArgs = getNArgs(args);
    for (int i = 0; i < nArgs; i++)
    {
        if (strcmp(args[i], "<") == 0)
        {
            return 1;
        }
    }
    return 0;
}

int containRedirectIn(char *args[])
{
    int nArgs = getNArgs(args);
    for (int i = 0; i < nArgs; i++)
    {
        if (strcmp(args[i], ">") == 0)
        {
            return 1;
        }
    }
    return 0;
}
void pipeExec(char *args[])
{

    int fd[2];
    char buf[200];
    pipe(fd);

    int index = getCharIndex(args, '|');
    args[index] = NULL;

    if (fork() == 0)
    {

        close(fd[0]);

        dup2(fd[1], STDOUT_FILENO);

        execvp(args[0], args);

        printf("left command failed!\n");
        destructArgs(args);
    }
    else
    {

        wait(NULL);
        close(fd[1]);

        dup2(fd[0], STDIN_FILENO);

        execvp(args[index + 1], &args[index + 1]);

        printf("grep command failed!\n");
        destructArgs(args);
    }
}
