//Pitchapa Phisutpichet 6580065
//Patrapee Maleevech 6580074
//Nitchayanin Thamkunanon 6580081
//Chanakan Boonchoo 6580128
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>

int msgID;
void signal_handler(int SIGNO){
  msgctl(msgID,IPC_RMID,NULL);
  exit(0);
}
struct a_msg
{
  long int msg_type;
  char data[BUFSIZ];
};

int main(int argc, char *argv[])
{
  int running = 1;
  struct a_msg a_msg;
  char buffer[BUFSIZ];
  long int msg_type = 0;

  if (argc < 2) 
    {
      fprintf(stderr, "Usage: %s User has to enter 1 or 2 as argv\n", *argv);
      exit(EXIT_FAILURE);
    }
 
  if (msgID == -1)
  {
    fprintf(stderr, "msgget failed\n");
    exit(EXIT_FAILURE);
  }
  argv++;
//roles : parent sends, child receives
//user 1. parent sends msg to type 1, child receieves msg from type 2 (user 2)
  if(strcmp(*argv, "1")==0)
  {
    msgID = msgget((key_t)6580081, 0666 | IPC_CREAT); 
    int child = fork();
    msg_type = 2;
    a_msg.msg_type = 1; //type 1
    while (running)
    {
      switch(child){
        case -1 : 
            perror("Forking failed"); 
            exit(EXIT_FAILURE); 
        case 0 : //child
            while (strncmp(a_msg.data, "end chat", 8) != 0) {
              msgrcv(msgID, (void *) &a_msg, BUFSIZ, msg_type, 0);
              printf("%s", a_msg.data);
            }
            running = 0; 
            pid_t parent = getppid();
          
            kill(getppid(),SIGTERM);
            raise(SIGTERM);
          break; 
        default : //parent
             signal(SIGTERM,signal_handler);
            while(strncmp(a_msg.data, "end chat", 8) != 0){
              fgets(buffer, BUFSIZ, stdin);
              strcpy(a_msg.data, buffer); //copy data from buffer to struct
              msgsnd(msgID, (void *) &a_msg, BUFSIZ, 0);
            }
            running = 0;
            kill(child,SIGTERM);
            raise(SIGTERM);
      }
    }

  }
  else if(strcmp(*argv, "2")==0)
    {
      msgID = msgget((key_t) 6580081, 0666 |IPC_CREAT); //CREATE/ACCESS MSGQ
      int child = fork();
      msg_type = 1;
      a_msg.msg_type = 2; 
      while (running)
      {
        switch(child){
          case -1 : 
              perror("Forking failed"); 
              exit(EXIT_FAILURE); 
          case 0 : //child
              while (strncmp(a_msg.data, "end chat", 8) != 0) {
                msgrcv(msgID, (void *) &a_msg, BUFSIZ, msg_type, 0);
                printf("%s", a_msg.data);
              }
              running = 0; 
              pid_t parent = getppid();
              // msgctl(msgID,IPC_RMID,NULL);
              kill(getppid(),SIGTERM);
              raise(SIGTERM);
            break; 
          default : //parent
               signal(SIGTERM,signal_handler);
              while(strncmp(a_msg.data, "end chat", 8) != 0){
                fgets(buffer, BUFSIZ, stdin);
                strcpy(a_msg.data, buffer); //copy data from buffer to struct
                msgsnd(msgID, (void *) &a_msg, BUFSIZ, 0);
              }
              running = 0;
              // msgctl(msgID,IPC_RMID,0);
              // msgctl(msgID,IPC_RMID,NULL);
              kill(child,SIGTERM);
              raise(SIGTERM);
        }
        }
      }
      
  exit(EXIT_SUCCESS);
}