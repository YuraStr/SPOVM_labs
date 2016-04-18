#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#define NAMEDPIPE_NAME "/tmp/my_named_pipe"
#define BUFSIZE 50

int main (int argc, char ** argv) {
  int fd;
  char buf[BUFSIZE];

  if ( (fd = open(NAMEDPIPE_NAME, O_WRONLY)) <= 0 ) {
    perror("open");
    return 1;
  }

  while(1) {
    memset(buf, '\0', BUFSIZE);
    printf ("Enter the message: ");
    scanf("%s", buf);
    write(fd, buf, strlen(buf));
    if (!strcmp(buf, "exit")) break;
  }

  close(fd);

  return 0;
}