#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <stdbool.h>

main(int argc, char **argv)
{
    int pid;
    int i;
    char* arg = malloc(sizeof(char) * 20);
    char* arguments[10];
    struct rusage usage;
    struct timeval time;
    struct timeval clockS;
    struct timeval clockE;
    struct timezone timeZone;
    char* prompt = "==>";
    bool running = true;
    gettimeofday(&clockS, &timeZone);

    if (argc > 1)
    {
        arg = argv[1];
        for (int i = 1; i < argc; i++)
        {
            arguments[i-1] = argv[i];
        }
        arguments[argc-1] = NULL;

    }else{
        while (1) {
            char input[128];
            printf("%s", prompt);
            fgets(input, 128, stdin);
            char *tok = strtok(input, " ,\n");
            int counter = 0;
            char **commands = malloc(sizeof(char *) * 128);

            while (tok != NULL) {
                commands[counter] = tok;
                counter++;
                tok = strtok(NULL, " ,\n");

            }
            commands[counter] = NULL;

            arg = commands[0];
            for (int i = 0; i < counter; i++) {
                arguments[i] = commands[i];
            }
            arguments[counter] = NULL;

            if (arguments[0] != NULL) {
                break;
            }
        }
    }
    if (!(strcmp(arg, "exit"))) {
        printf("Ending the Program. NOW!\n");
        exit(0);
    }
    if ((!(strcmp(arg, "set"))) && (!(strcmp(arguments[1], "prompt"))) && (!(strcmp(arguments[2], "=")))) {
        printf("setting new prompt:\n");
        prompt = arguments[3];
        while (1) {
            char input[128];
            printf("%s", prompt);
            fgets(input, 128, stdin);
            char *tok = strtok(input, " ,\n");
            int counter = 0;
            char **commands = malloc(sizeof(char *) * 128);

            while (tok != NULL) {
                commands[counter] = tok;
                counter++;
                tok = strtok(NULL, " ,\n");

            }
            commands[counter] = NULL;

            arg = commands[0];
            for (int i = 0; i < counter; i++) {
                arguments[i] = commands[i];
            }
            arguments[counter] = NULL;

            if (arguments[0] != NULL) {
                break;
            }
        }
    }
    if ((pid = fork()) < 0)
    {
        fprintf(stderr, "Fork error\n");
        exit(1);
    }
    else if (pid == 0)
    { 
        execvp(arg,arguments);
        exit(0);
    }
    else
    {
        /* parent */
        wait(NULL);
        getrusage(RUSAGE_CHILDREN, &usage);
        printf("The Amount of CPU time was %d milleseconds\n", usage.ru_utime.tv_usec);
        gettimeofday(&clockE, &timeZone);
        printf("The entire process took %d milleseconds\n", (clockE.tv_usec-clockS.tv_usec));
        printf("the number of times the process was preempted involuntarily is %d\n",usage.ru_nivcsw);
        printf("the number of times the process gave up the CPU voluntarily is %d\n",usage.ru_nvcsw);
        printf("the number of major page faults, which require disk I/O,is %d\n",usage.ru_majflt);
        printf("the number of minor page faults, which could be satisfied without disk I/O is %d\n",usage.ru_minflt);
        printf("the maximum resident set size used,in kilobytes is %d\n",usage.ru_maxrss);
	}
}
