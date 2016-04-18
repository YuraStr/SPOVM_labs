#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#define NAMEDPIPE_NAME "/tmp/my_named_pipe"
#define BUFSIZE 50

int main (int argc, char ** argv) {
  pid_t pid;
  int fd, len;
  char buf[BUFSIZE];

  if (mkfifo(NAMEDPIPE_NAME, 0777)) {
    perror("mkfifo");
    return 1;
  }
  printf("%s is created\n", NAMEDPIPE_NAME);

  if ((fd = open(NAMEDPIPE_NAME, O_RDONLY)) <= 0 ) {
    perror("open");
    return 1;
  }

  while(1) {
    memset(buf, '\0', BUFSIZE);
    len = read(fd, buf, BUFSIZE-1);
    if (!strcmp(buf, "exit")) break;
    printf("Recieved: %s\n", buf);
  }
  

  /*if ( (fd = open(NAMEDPIPE_NAME, O_RDONLY)) <= 0 ) {
    perror("open");
    return 1;
  }
  printf("%s is opened\n", NAMEDPIPE_NAME);

  do {
    memset(buf, '\0', BUFSIZE);
    if ( (len = read(fd, buf, BUFSIZE-1)) <= 0 ) {
      perror("read");
      close(fd);
      remove(NAMEDPIPE_NAME);
      return 0;
    }
    printf("Incomming message (%d): %s\n", len, buf);
  } while ( 1 );*/


  close(fd);
  remove(NAMEDPIPE_NAME);

  return 0;
}