#include "builtin.h"
#include "executor.h"
#include "argumentHandle.h"

void mySignal(int signo);

int main()
{

    int should_run = 1;
    while (should_run == 1)
    {
        signal(SIGTSTP, mySignal);
        signal(SIGINT, mySignal);

        int status = 1;

        char *args[MAX_ARGS];
        initShell(args);

        printf(GREEN);
        printf("OSH>> ");
        printf("\033[0m");
        fflush(stdout);

        input(args);

        if (strcmp(args[getNArgs(args) - 1], "&") == 0)
        {
            status = 0;
            args[getNArgs(args) - 1] = NULL;
        }

        if (!isPipe(args) && execBuiltin(args, getNArgs(args)))
        {
        }
        else
        {

            int pid = fork();
            if (pid > 0)
            {

                if (status == 0)
                    continue;

                wait(NULL);
            }
            else
            {

                executeArgs(args);

                if (args[0][0] != '\0')
                    printf("error, command does not exist\n");

                destructArgs(args);
                exit(1);
            }
        }
    }
    return 0;
}

void mySignal(int signo)
{
    if (signo == SIGINT)
    {
        printf("Interupt signal\n");
    }
    else if (signo == SIGTSTP)
    {
        printf("Stop signal\n");
    }
    printf(GREEN);
    printf("OSH>> ");
    printf("\033[0m");
    fflush(stdout);
}