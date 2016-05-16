#ifdef __linux__
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#elif _WIN32
#include <stdio.h>
#include <windows.h>
#endif

#ifdef __linux__
#define NAMEDPIPE_NAME "/tmp/my_named_pipe"
#define BUFSIZE 50
#elif _WIN32
#define NAMEDPIPE_NAME "\\\\.\\pipe\\Pipe"
#define BUFSIZE 80
#endif

int main (int argc, char ** argv) {
#ifdef __linux__ 
  int fd;
  char buf[BUFSIZE];

  if ( (fd = open(NAMEDPIPE_NAME, O_WRONLY)) <= 0 ) {
    perror("open");
    return 1;
  }

  while(1) {
    memset(buf, '\0', BUFSIZE);
    printf ("Enter message: ");
    scanf("%s", buf);
    write(fd, buf, strlen(buf));
    if (!strcmp(buf, "exit")) break;
  }

  close(fd);

#elif _WIN32
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
                strlen(buf),
                &dwWritten,
                NULL);
      if (!strcmp(buf, "exit")) break;
    }
    CloseHandle(hPipe);
  }
#endif
  return 0;
}