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

void submit_job(char *arr[], int size, char *command)
{
    printf("Actual command: %s\n", command);

    
    add_job_to_queue(arr, command);
    run_job_scheduler();
}

void run_job_scheduler()
{

    struct Job *job = get_front_queue();
    if (job != NULL)
    {
        if (running_jobs < max_jobs)
        {
            exec_job(job);
            running_jobs++;
        }
        else
        {
            printf("Jobs is in queue, all cores are occupied, will start the job once any of the cores are freed up\n");
        }
    }
}

void process_command(char *command)
{
    char *token = strtok(command, " ");

    int i = 0;
    char *arr[100];
    char actualCommand[1000] = "";

    if (strcmp(SUBMIT_CMD, token) == 0)
    {
        while (token != NULL)
        {
            token = strtok(NULL, " ");
            arr[i++] = token;
            if (token != NULL)
            {
                strcat(actualCommand, token);
                strcat(actualCommand, " ");
            }
        }
        submit_job(arr, i, actualCommand);
    }
    else if (strcmp(SHOW_JOBS, token) == 0)
    {
        display_queue();
    }
    else if (strcmp(SUBMIT_HISTORY, token) == 0)
    {
        display_completed();
    }
}

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        printf("Usage: %s <max_jobs>\n", argv[0]);
        exit(1);
    }

    max_jobs = atoi(argv[1]);
    char command[1000];

    do
    {
        printf("Enter command > ");
        
        gets(command);

        process_command(command);
    } while (strcmp(command, "exit") != 0);
    return 0;
}




