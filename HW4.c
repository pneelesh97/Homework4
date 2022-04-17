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
