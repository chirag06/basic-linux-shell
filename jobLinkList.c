#include "jobLinkList.h"

struct Job *head=NULL;
struct Job *ScanList(int searchID){
  struct Job *previous, *current;
  previous = head;
  current = head->next;
  /* Traverse until ID >= searchID */
  while ((current!= NULL)
      && (current->jobID != searchID)){
        previous = current;
        current = current->next;

      }
    return previous;
}

// Add
void AddList(int jobID, char *jobName){
  struct Job *prevNode, *nextNode;
  struct Job* newNode = (struct Job*)malloc(sizeof(struct Job));
  newNode->jobID = jobID;
  strcpy( newNode->name, jobName);

  /* initialize node with new car info */
  //prevNode = ScanList(newNode->jobID);
  //nextNode = prevNode->next;
  if ((head != NULL)){
      newNode->next = head;
  }
      head = newNode;
}

//delete
int DelList(int jobID_del){
  //int jobID_del;
  struct Job *delNode, *prevNode;
  //printf("Enter Job ID of car to delete:\n");
  //scanf("%d", &jobID_del);
  if(head->jobID == jobID_del){
    head = head->next;
    return 1;
  }
  prevNode = ScanList(jobID_del);
  delNode = prevNode->next;
  if ((delNode != NULL)
      && (delNode->jobID == jobID_del)){
    prevNode->next = delNode->next;
    free(delNode);
  }else {
      printf("Job not found in database.\n");
  }
  return 1;
}

void print_list(){
    struct Job * current = head;
    while (current != NULL) {
        printf("%d\t%s\n", current->jobID, current->name);
        current = current->next;
    }
}
/*
int main() {

  AddList(123,"hello");
  AddList(456,"t");
  AddList(789,"tas");
  AddList(12,"as");
  print_list();
  DelList();
  print_list();
  return 1;
}
*/
