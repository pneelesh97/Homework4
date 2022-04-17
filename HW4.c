#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include "queue.h"

#define SUBMIT_CMD "submit"
#define SHOW_JOBS "showjobs"
#define SUBMIT_HISTORY "submithistory"

int max_jobs = 0;
int running_jobs = 0;

void run_job_scheduler();
void exec_job(struct Job *job);
void submit_job(char *arr[], int size, char *command);
void process_command(char *command);

void exec_job(struct Job *job)
{
    time_t t = time(NULL);
    pid_t pid;
    int status;
    int tPrintLimit = 128;

    int fdout, fderr;
    char outFName[100], errFName[100];
   
    char *ppidStr;

    pid = fork();

    
    if (asprintf(&ppidStr, "%d", getpid()) == -1)
    {
        perror("asprintf");
    }
    else
    {
       
        strcpy(outFName, ".out");
        strcat(ppidStr, outFName);
        
        strcpy(outFName, "");
        strcpy(outFName, ppidStr);
        free(ppidStr);
    }

    if (asprintf(&ppidStr, "%d", getpid()) == -1)
    {
        perror("asprintf");
    }
    else
    {
        
        strcpy(errFName, ".err");
        strcat(ppidStr, errFName);
       
        strcpy(errFName, "");
        strcpy(errFName, ppidStr);
        free(ppidStr);
    }
    job->pid = getpid();
    job->status = RUNNING;
    strcpy(job->startTime, ctime(&t));
   

    if ((fdout = open(outFName, O_CREAT | O_APPEND | O_WRONLY, 0755)) == -1)
    {
        printf("Error opening file %s for output\n", outFName);
        exit(-1);
    }
    
    if ((fderr = open(errFName, O_CREAT | O_APPEND | O_WRONLY, 0755)) ==
        -1)
    {
        printf("Error opening file %s for error\n", errFName);
        exit(-1);
    }

    if (pid == 0)
    { /* this is child process */
        dup2(fdout, 1);
        dup2(fderr, 2);
        

        if (execvp(job->argv[0], &job->argv[0]) < 0)
        { 
            printf("*** ERROR: exec failed\n");
            exit(1);
        }
    }
    else if (pid > 0)
    { 
        write(fdout, "Wait for the child process to terminate\n", tPrintLimit);
        wait(&status); 
        if (WIFEXITED(status))
        { 
            job->status = SUCCESS;
            strcpy(job->endTime, ctime(&t));
            job_completed_by_index(job->index);
            write(fdout, "Child process exited with status = %d\n", WIFEXITED(status));
        }
        else
        { 
            job->status = FAILED;
            strcpy(job->endTime, ctime(&t));
            job_completed_by_index(job->index);

            write(fdout, "Child process did not terminate normally!\n", tPrintLimit);
            
        }
    }
    else
    {                   
        perror("fork"); 
        exit(EXIT_FAILURE);
    }
    close(fdout);
    close(fderr);

    write(fdout, "[%ld]: Exiting program .....\n", (long)getpid());

    run_job_scheduler();
}


