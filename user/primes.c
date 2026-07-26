#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int i[1];
  int prime;
  int flag;
  int p[2];
  int q[2];
  pipe(p);

  int pid = fork();
  if (pid != 0) {
    close(p[0]);
    for (*i = 2; *i < 36; (*i)++) {
      write(p[1], i, 4);
    }
    close(p[1]);
    wait(0);
    exit(0);
  }

  while (pid == 0) {
    read(p[0], i, 4);
    prime = *i;
    fprintf(1, "prime %d\n", prime);

    close(p[1]);
    close(q[1]);
    pipe(q);

    //找到下一个素数
    while ((flag = read(p[0], i, 4)) || flag != 0) {
      if (*i % prime != 0) {
        write(q[1], i, 4);  //q[1]接收到素数了
        pid = fork();
        break;
      }
    }

    if (flag == 0) {
      close(p[0]);
      close(p[1]);
      close(q[0]);
      close(q[1]);
      exit(0);
    }


    if (pid != 0) {
      while (read(p[0], i, 4) != 0) {
        if (*i % prime != 0) {
          write(q[1], i, 4);
        }
      }
      close(p[0]);
      close(p[1]);
      close(q[0]);
      close(q[1]);
      wait(0);
      exit(0);
    }

    if (pid == 0) {
      close(p[0]);
      p[0] = q[0];
    }
  }
}
