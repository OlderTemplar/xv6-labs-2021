#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int p[2];
  int pid;


  pipe(p);
  if (write(p[1], " ", 1) < 0) {
    fprintf(2, "Error when parent is writing\n");
    exit(1);
  }
  pid = fork();

  // child
  if (pid == 0) {

    if (read(p[0], 0, 1) < 0) {
      fprintf(2, "Error when child is reading\n");
      exit(1);
    }

    pid = getpid();
    fprintf(1, "%d: received ping\n", pid);

    if (write(p[1], " ", 1) < 0) {
      fprintf(2, "Error when child is writing\n");
      exit(1);
    }
    close(p[0]);
    close(p[1]);
    exit(0);
  }

  // parent
  wait(0);
  if (read(p[0], 0, 1) < 0) {
    fprintf(2, "Error when parent is reading\n");
    exit(1);
  }
  pid = getpid();
  fprintf(1, "%d: received pong\n", pid);
  close(p[0]);
  close(p[1]);
  exit(0);
}
