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
