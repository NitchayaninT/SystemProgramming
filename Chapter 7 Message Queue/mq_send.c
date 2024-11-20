#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct a_msg
{
  long int msg_type;
  char data[BUFSIZ];
};

int main(void)
{
  int running = 1, msgID;
  struct a_msg a_msg;
  char buffer[BUFSIZ];

  msgID = msgget((key_t) 1234, 0666 |IPC_CREAT);
  //when theres error in creating msg queue
if (msgID == -1)
{
  fprintf(stderr, "msgget failed\n");
  exit(EXIT_FAILURE);
}
  //otherwise, asking user for data
  while (running)
  {
    printf("Enter data: ");
    fgets(buffer, BUFSIZ, stdin);
    a_msg.msg_type = 1; //type 1
    strcpy(a_msg.data, buffer); //copy data from buffer to struct
  if (msgsnd(msgID, (void *) &a_msg, BUFSIZ, 0) == -1) //if theres error
  {
    fprintf(stderr, "msgsnd failed\n");
    exit(EXIT_FAILURE);
  }
  if (strncmp(buffer, "end", 3) == 0)//compare if the data is equal to end (means user type end to stdin)
    running = 0;
  }
    exit(EXIT_SUCCESS);
  }