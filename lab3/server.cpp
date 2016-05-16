#ifdef __linux__
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#define NAMEDPIPE_NAME "/tmp/my_named_pipe"
#define BUFSIZE 50

#elif _WIN32
#include <Windows.h>
#include <stdio.h>

#define NAMEDPIPE_NAME "\\\\.\\pipe\\Pipe"
#define BUFSIZE 80
#endif

int main (int argc, char ** argv) {
#ifdef __linux__
  pid_t pid;
  int fd, len;
  char buf[BUFSIZE];

  if (mkfifo(NAMEDPIPE_NAME, 0777)) {
    perror("mkfifo");
    return 1;
  }
  printf("Server: start\n");

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
  
  close(fd);
  remove(NAMEDPIPE_NAME);
#elif _WIN32
  HANDLE hPipe;
  char buffer[BUFSIZE];
  DWORD dwRead;

  hPipe = CreateNamedPipe(TEXT(NAMEDPIPE_NAME),
                          PIPE_ACCESS_DUPLEX | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE,
                          PIPE_WAIT,
                          1,
                          1024 * 16,
                          1024 * 16,
                          NMPWAIT_USE_DEFAULT_WAIT,
                          NULL);

  printf("Server: start\n");
  if (ConnectNamedPipe(hPipe, NULL) != FALSE) {
  printf("New pipe was connected\n");
    while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE) {
      buffer[dwRead] = '\0';
      printf("Recieved: %s\n", buffer);
      if (!strcmp(buffer, "exit")) break;
    }
  }
  DisconnectNamedPipe(hPipe);

  CloseHandle(hPipe);
#endif
  return 0;
}