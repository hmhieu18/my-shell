#include "builtin.h"

void exitShell(char **args, int nArgs)
{
    if (nArgs > 1)
    {
        fprintf(stderr, "\nExit takes 0 arg");
    }
    else
    {
        printf("Exec builtin %s\n", args[0]);
        printf("See you again\n");
        exit(EXIT_SUCCESS);
    }
}

int myCompare(const void *a, const void *b)
{
    return strcmp(*(const char **)a, *(const char **)b);
}

char **sort(char *arr[], int n)
{
    qsort(arr, n, sizeof(const char *), myCompare);
    return arr;
}

void cdHelper(char *curDir, char *newDir)
{
    char *path = strdup(newDir);
    char directory[MAX_ARG_LENGTH];
    getcwd(directory, sizeof(directory));
    if (strcmp(path, "..") == 0)
    {
        chdir(directory);
        return;
    }
    strcat(directory, "/");
    if (strcmp(path, "-"))
    {
        chdir(curDir);
        return;
    }
    if (strstr(path, "/"))
    {
        if (chdir(path) != 0)
        {
            chdir(path);
        }
        return;
    }
    strcat(directory, path);
    if (chdir(directory) != 0)
    {
        perror("cd");
    }
    else
    {
        chdir(directory);
    }
}

void changeDirectory(char **args, int nArgs)
{
    char directory[MAX_ARG_LENGTH];
    getcwd(directory, sizeof(directory));
    char *path = strdup(directory);
    if (nArgs < 2)
    {
        printf("Exec builtin %s\n", args[0]);
        strcpy(path, directory);
        cdHelper(getenv("HOME"), path);
    }
    else if (nArgs == 2)
    {
        printf("Exec builtin %s\n", args[0]);
        strcpy(path, directory);
        if (chdir(args[1]) != -1 || args[1] == "-")
        {
            strcpy(path, directory);
        }
        cdHelper(args[1], path);
    }
    else
    {
        fprintf(stderr, "Too many arguments\n");
    }
}

char **ls(int *n)
{

    static char *output[500];
    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    struct stat filestat;

    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
                continue;
            stat(dir->d_name, &filestat);
            output[*n] = dir->d_name;
            *n += 1;
            if (S_ISDIR(filestat.st_mode))
                printf(BLUE "%s " BLUE, dir->d_name);
            else if (filestat.st_mode & X_OK)
                printf(GREEN "%s " GREEN, dir->d_name);
            else
                printf(NORMAL "%s " NORMAL, dir->d_name);
        }
        printf("%s\n", NORMAL);
    }

    closedir(d);
    return output;
}

char *getPwd()
{
    static char path[1001];
    if (getcwd(path, sizeof(path)) != NULL)
    {
        printf("%s\n", path);
        return path;
    }
    else
    {
        perror("pwd error");
        return "";
    }
}