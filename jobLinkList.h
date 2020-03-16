#ifndef JOBLINKLIST
#define JOBLINKLIST


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

void AddList(int jobID, char *jobName);
int DelList(int jobID_del);
void print_list();

struct Job {
  int jobID;
  char name[20];
  struct Job *next;
};
#endif
