#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

int jobCount = 0;

struct Job *front = NULL;
struct Job *rear = NULL;

struct Job *frontCompleted = NULL;
struct Job *rearCompleted = NULL;

struct Job *get_front_queue()
{
    return front;
}

void add_completed_job_to_nqueue(struct Job *completed)
{
    completed->next = NULL;
    if (rearCompleted == NULL)
    {
        frontCompleted = completed;
        rearCompleted = completed;
    }
    else
    {
        rearCompleted->next = completed;
        rearCompleted = rearCompleted->next;
    }
}

void job_completed_by_index(int index)
{
    // pick from the main queue and add it to completed queue
    if (front == NULL)
    {
        printf("Queue is null");
        return;
    }

    struct Job *temp;
    struct Job *previous = NULL;

    // start from front
    temp = front;

    while (temp->index != index)
    {
        if (temp->next == NULL)
        {
            printf("Invalid index");
            return;
        }
        else
        {
            previous = temp;
            temp = temp->next;
        }
    }
    if (temp == front)
    {
        front = front->next;
    }
    else
    {
        previous->next = temp->next;
    }

    // printf("\n\n%d deleted", temp->pid);
    add_completed_job_to_nqueue(temp);
    // free(temp);
}

void update_pid_job(int index, int pid)
{
}

struct Job *add_job_to_queue(char **argv, char *command)
{
    struct Job *nptr = malloc(sizeof(struct Job));
    nptr->index = ++jobCount;
    nptr->pid = -1;
    strcpy(nptr->command, command);
    nptr->argv = argv;
    // printf("Submitted command: %s\n", command);

    nptr->status = WAITING;

    nptr->next = NULL;
    if (rear == NULL || front == NULL)
    {
        front = nptr;
        rear = nptr;
    }
    else
    {
        rear->next = nptr;
        rear = rear->next;
    }
    display_queue();
    return nptr;
}

void display_queue()
{
    struct Job *temp;
    temp = front;
    printf("\njobid\tcommand\t\t\tstatus\n");
    while (temp != NULL)
    {
        printf("%d\t%s\t%s\n", temp->index, temp->command, temp->status);
        temp = temp->next;
    }
}

void display_completed()
{
    struct Job *temp;
    temp = frontCompleted;
    printf("\njobid\tcommand\t\t\tstartTime\tendTime\tstatus\n");
    while (temp != NULL)
    {
        printf("%d\t%s\t%s\t%s\t%s\n", temp->index, temp->command, temp->startTime, temp->endTime, temp->status);
        temp = temp->next;
    }
}

void remove_front_queue()
{
    if (front == NULL)
    {
        // printf("\n\nqueue is empty \n");
    }
    else
    {
        struct Job *temp;
        temp = front;
        front = front->next;
        // printf("\n\n%d deleted", temp->pid);
        free(temp);
    }
}
