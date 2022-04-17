#ifndef __QUEUE_H__
#define __QUEUE_H__

#define RUNNING "Running"
#define WAITING "Waiting"
#define SUCCESS "Success"
#define FAILED "Failed"

struct Job
{
    int index;
    pid_t pid;
    char command[1000];
    char **argv;
    char *status;
    char startTime[128];
    char endTime[128];
    struct Job *next;
};

void display_queue();
void display_completed();
struct Job *add_job_to_queue(char **argv, char *command);
void update_pid_job(int index, int pid);
void remove_front_queue();
void job_completed_by_index(int pid);
void add_completed_job_to_nqueue(struct Job *completed);
struct Job *get_front_queue();
#endif