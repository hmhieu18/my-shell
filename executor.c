#include "executor.h"
void executeArgs(char *args[])
{
    if (isPipe(args))
    {
        pipeExec(args);
    }
    else if (containRedirectOut(args))
    {
        redirectingOut(args);
    }
    else if (containRedirectIn(args))
    {
        redirectingIn(args);
    }
    else
    {

        execvp(args[0], args);
    }
}
void redirectingIn(char *args[])
{
    int indexCmd = getCharIndex(args, '>');
    args[indexCmd] = NULL;

    int fileDescription = open(args[indexCmd + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fileDescription < 0)
    {
        printf("Unable to open file\n");
        return;
    }

    dup2(fileDescription, STDOUT_FILENO);
    execvp(args[0], args);

    destructArgs(args);
    close(fileDescription);
}
void redirectingOut(char *args[])
{
    printf("in redireting\n");
    int id = getCharIndex(args, '<');
    args[id] = NULL;

    int fileDescription = open(args[id + 1], O_RDONLY);
    if (fileDescription < 0)
    {

        destructArgs(args);
        exit(0);
    }
    else
    {
        dup2(fileDescription, STDIN_FILENO);
        execvp(args[0], args);

        destructArgs(args);
        close(fileDescription);
    }
}

void execExit(char *args[], int nArgs)
{
    exitShell(args, nArgs);
    destructArgs(args);
}

void execCD(char *args[], int nArgs)
{
    if (containRedirectOut(args))
    {
        int cnt = 0;
        int *p = &cnt;
        char **res = readFromFile(args[2], p);
        if (cnt > 0)
        {
            strcpy(args[nArgs - 2], res[0]);
            args[nArgs - 1] = NULL;
        }
        else
        {
            perror("FILE ERROR");
        }
        nArgs--;
    }
    changeDirectory(args, nArgs);
    destructArgs(args);
}

void execPWD(char *args[], int nArgs)
{
    char *pwd = getPwd();

    if (containRedirectIn(args))
    {
        int out = open(args[nArgs - 1], O_RDWR | O_CREAT | O_TRUNC, 0600);
        if (-1 == out)
        {
            perror("opening cout.log");
            return 255;
        }
        int save_out = dup(fileno(stdout));

        if (-1 == dup2(out, fileno(stdout)))
        {
            perror("cannot redirect stdout");
            return 255;
        }

        puts(pwd);

        fflush(stdout);
        close(out);

        dup2(save_out, fileno(stdout));
        close(save_out);
    }

    destructArgs(args);
}

void execSort(char *args[], int nArgs)
{
    int cnt = 0;
    int *p = &cnt;
    if (nArgs >= 2)
    {
        char **res;
        if (nArgs == 2)
        {
            res = readFromFile(args[1], p);
        }
        else if (nArgs == 3 && containRedirectOut(args))
        {
            res = readFromFile(args[2], p);
        }

        sort(res, cnt);
        for (int i = 0; i < cnt; i++)
        {
            if (res[i][strlen(res[i]) - 1] == '\n')
                printf("%s", res[i]);
            else
                printf("%s\n", res[i]);
        }
        printf("\n");
        destructArgs(args);
    }
    else
    {
        perror("SYNTAX ERROR");
    }
}

void execLS(char *args[], int nArgs)
{
    int x = 0;
    int *n = &x;

    char **result = ls(n);
    if (containRedirectIn(args))
    {
        writeToFile(result, x, args[nArgs - 1]);
    }

    destructArgs(args);
}

void execHistory(char *args[], int nArgs, char path[])
{
    FILE *file = fopen(path, "r");
    char *list[10];
    int i = 0;
    if (file)
    {

        char *s = calloc(MAX_CMD_LENGTH, sizeof(char));
        fgets(s, MAX_CMD_LENGTH, file);
        while (fgets(s, MAX_CMD_LENGTH, file))
        {
            if (nArgs == 1)
                printf("%d: %s", i + 1, s);
            list[i] = s;
            s = calloc(MAX_CMD_LENGTH, sizeof(char));
            i++;
        }

        fclose(file);
        printf("\n");
    }
    if (nArgs == 3 && strcmp(args[nArgs - 2], ">") == 0)
    {
        file = fopen(args[nArgs - 1], "w");
        if (file)
        {
            for (int it = 0; it < i; it++)
            {
                fprintf(file, "%d: %s", it + 1, list[it]);
                free(list[it]);
            }
            fclose(file);
        }
    }
}
bool execBuiltin(char *args[], int nArgs, char path[])
{
    int noBuiltins = 6, i, switchOwnArg = -1;
    char *builtinList[noBuiltins];
    char *username;

    builtinList[0] = "exit";
    builtinList[1] = "cd";
    builtinList[2] = "pwd";
    builtinList[3] = "sort";
    builtinList[4] = "ls";
    builtinList[5] = "history";

    for (i = 0; i < noBuiltins; i++)
    {
        if (strcmp(args[0], builtinList[i]) == 0)
        {
            switchOwnArg = i;
            break;
        }
    }

    switch (switchOwnArg)
    {
    case 0:
        execExit(args, nArgs);
        return true;
    case 1:
        execCD(args, nArgs);
        return true;
    case 2:;
        execPWD(args, nArgs);
        return true;
    case 3:;
        execSort(args, nArgs);
        return true;
    case 4:;
        execLS(args, nArgs);
        return true;
    case 5:
        execHistory(args, nArgs, path);
        return true;
    default:
        break;
    }

    return false;
}

void writeToFile(char *input[], int x, char *fileName)
{
    int out = open(fileName, O_RDWR | O_CREAT | O_TRUNC, 0600);
    if (-1 == out)
    {
        perror("opening error");
        return;
    }
    int outPut = dup(fileno(stdout));

    if (-1 == dup2(out, fileno(stdout)))
    {
        perror("cannot redirect stdout");
        return;
    }

    for (int i = 0; i < x; i++)
    {
        puts(input[i]);
    }

    fflush(stdout);
    close(out);

    dup2(outPut, fileno(stdout));
    close(outPut);
}
char **readFromFile(char *fileName, int *n)
{
    int length = 1000;
    char **input = (char **)calloc(length, sizeof(char *));
    for (int i = 0; i < length; i++)
    {
        input[i] = '\0';
    }
    FILE *readFile;
    char *s = calloc(1000, sizeof(char));
    readFile = fopen(fileName, "r");

    if (readFile == NULL)
    {
        perror("Unable to open file\n");
    }
    else
    {

        while (fgets(s, 1000, readFile))
        {
            input[*n] = s;
            input[*n][strlen(s)] = '\0';
            s = calloc(1000, sizeof(char));
            *n += 1;
        }

        fclose(readFile);
    }
    return input;
}