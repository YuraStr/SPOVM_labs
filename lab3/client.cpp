#ifdef __linux__
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define NAMEDPIPE_NAME "/tmp/my_named_pipe"
#define BUFSIZE 50

int main (int argc, char ** argv) {
  int fd;
  char buf[BUFSIZE];

  if ( (fd = open(NAMEDPIPE_NAME, O_WRONLY)) <= 0 ) {
    perror("open");
    return 1;
  }

  printf("Client: start\n");
  while(1) {
    memset(buf, '\0', BUFSIZE);
    printf ("Enter message: ");
    scanf("%s", buf);
    write(fd, buf, strlen(buf));
    if (!strcmp(buf, "exit")) break;
  }

  close(fd);

  return 0;
}
#elif _WIN32
#include <stdio.h>
#include <windows.h>

#define NAMEDPIPE_NAME "\\\\.\\pipe\\Pipe"
#define BUFSIZE 80

int main(void)
{
  HANDLE hPipe;
  DWORD dwWritten;
  char buf[80];

  hPipe = CreateFile(TEXT(NAMEDPIPE_NAME), 
                     GENERIC_READ | GENERIC_WRITE, 
                     0,
                     NULL,
                     OPEN_EXISTING,
                     0,
                     NULL);
  if (hPipe != INVALID_HANDLE_VALUE) {
    printf("Client: start\n");
    while (1) {
      printf("Enter message: ");
      scanf("%s", buf);
      WriteFile(hPipe,
                buf,
                strlen(buf),   // = length of string + terminating '\0' !!!
                &dwWritten,
                NULL);
      if (!strcmp(buf, "exit")) break;
    }
    CloseHandle(hPipe);
  }

  return (0);
}
#endif