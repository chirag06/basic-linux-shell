#include "jobLinkList.h"

char cwd[256];
pid_t pid,wpid;
int status;

void blazerlog(int readOnly, char *output){
  FILE *fptr;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;
  char filename[100];
  strcpy(filename,cwd);
  strcat(filename,"/blazersh.log");
  fptr = fopen(filename, "arw+");
  if(fptr == NULL){
    printf("Error reading log file\n");
    exit (-1);
  }
  if(readOnly==1){
    while ((read = getline(&line, &len, fptr)) != -1) {
        printf("%s", line);
    }
  }else{
    fprintf(fptr, "%s", output);
  }
  fclose(fptr);
}
void runCmd(char **cmd,int arrayL){
  int fdout, fdin;
  if(strcmp(cmd[0], "list") == 0) {
      char *cmd[2]={"ls",NULL};
    	runCmd(cmd, arrayL);
  } else if(strcmp(cmd[0], "cd") == 0) {
    	char s[100];
    	if (chdir(cmd[1]) != 0)
      		perror(0);
    	printf("%s\n", getcwd(s, 100));
  } else if(strcmp(cmd[0], "history") == 0) {
    blazerlog(1,"");
  } else if(strcmp(cmd[0], "help") == 0) {
    printf("BLAZER shell commands: \n");
    printf("list\t-\tlist all the files in the current directory. \
    \n\t\tThe default directory is the directory where the shell \
    \n\t\tprogram was invoked or the directory specified by the PWD \
    \n\t\tenvironment variable.\n");
    printf("cd\t-\tchange the current directory to the default directory \
    \n\t\twould be the directory where the shell program was invoked.\n");
    printf("help\t-\tDisplay the internal commands and a short description on how to \
    \n\t\tuse them\n");
    printf("history\t-\tdisplay all the previous command entered into the shell program\n");
    printf("quit\t-\texit the shell\n");
  } else if(strcmp(cmd[0], "quit") == 0) {
    	exit(-1);
  }else if(strcmp(cmd[0], "continue")==0){
    int result;
    if(cmd[1]== NULL){
      fprintf(stderr, "blazersh: expected argument to \"continue\"\n");
    }else if((result = kill(atoi(cmd[1]), SIGCONT)) != 0){
      perror("blazersh"); // throw error if dir doesn't exist
    }
    if (result == 0){
      printf("Process %s started.\n", cmd[1]);
      DelList(atoi(cmd[1]));
    }
  }else if(strcmp(cmd[0], "jobs") == 0){
    print_list();
  }
  else{
    int status,k=0;

    pid = fork();
    if (pid == 0) { /* this is child process */
      int j=0, flag=0;
      char *cmd_cp[100];
      for(;j<arrayL;j++){
        if(strcmp(cmd[j], "<") == 0 || strcmp(cmd[j], "<\n")==0 || strcmp(cmd[j], ">") == 0 || strcmp(cmd[j], ">\n")==0){
          flag = 1;
          if(arrayL>j+1){
            if(strcmp(cmd[j], "<")==0){
              if (((fdin = open(cmd[j+1], O_RDONLY, 0755)) == -1)) {
                printf("Error opening file %s for output\n",cmd[j]);
                exit(-1);
              }
              dup2(fdin, 0);
            }else if(strcmp(cmd[j], ">")==0){
              if (((fdout = open(cmd[j+1], O_CREAT | O_APPEND | O_WRONLY, 0755)) == -1)) {
                printf("Error opening file %s for output\n",cmd[j]);
                exit(-1);
              }
              dup2(fdout, 1);
            }
          }else{
            printf("Filename not given\n");
            return;
          }
        }else if(flag==0){
          cmd_cp[k] = cmd[j];
          k++;
        }else if(flag==1){
          flag = 0;
        }
      }
      cmd_cp[k]='\0';
      execvp(cmd_cp[0], cmd_cp);
      printf("Command not found!!!\nTry help command for more information.\n");
      exit(-1);
      } else if (pid > 0) { /* this is the parent process */

        signal(SIGINT, SIG_IGN);
        signal(SIGTSTP, SIG_IGN);
        wpid = waitpid(pid, &status, WUNTRACED);
        if (WIFEXITED(status)) { /* child process terminated normally */
            //printf("Child process exited with status = %d\n", WEXITSTATUS(status));
        } else if(WIFSTOPPED(status)) {
            printf("Process with pid=%d has been STOPPED\n", pid);
            AddList(pid, cmd[0]);
        } else if(WTERMSIG(status) == 2) {
            printf("Process with pid=%d has been INTERRUPTED...\n", wpid);
        } else { /* child process did not terminate normally */
            printf("Child process did not terminate normally!\n");
        }
        signal(SIGINT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);

    } else { /* we have an error */
        perror("fork"); /* use perror to print the system error message */
        exit(EXIT_FAILURE);
    }
  }
}

char *readInput(void){
  static char input[100];
  printf("blazersh>");
  fgets(input, 256, stdin);
  blazerlog(0,input);
  fflush(stdin);
  return input;
}

int main(int argc, char *argv[]){
  char * lineStr = NULL;
  char * line = NULL;
  size_t len = 0;
  FILE *fp, *fw;
  ssize_t read;
  getcwd(cwd, sizeof(cwd));
  if (argc < 2) {
    while(1){
      char *line = readInput();
      if(line[0]=='\n'){
        continue;
      }
      size_t len = strlen(line);
      char *array[10];
      char *lineCopy = line;
      int i=0;
      if (len > 0 && line[len-1] == '\n') {
        line[--len] = '\0';
      }
      while ((array[i++] = strtok_r(lineCopy, " ", &lineCopy)));
      runCmd(array, i-1);
    }
  }else{
    fp = fopen(argv[1], "r");
    fw = fopen("output.log", "w+");
    if (fp == NULL)
        exit(EXIT_FAILURE);
    while ((read = getline(&line, &len, fp)) != -1) {
      size_t len = strlen(line);
      lineStr = line;
      if (len > 0 && line[len-1] == '\n') {
        line[--len] = '\0';
      }
      char *array[10];
      int i=0,j=0;
      char *token, startTime[100], *endTime, stringLog[100];
      while ((array[i++] = strtok_r(line, " ", &line)));
      runCmd(array, i-1);
    }
  }
}
