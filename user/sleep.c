#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char* argv[])
{
  int i;

  if (argc < 2) {
    fprintf(2, "Lack an argument\n");
    exit(1);
  }

  if (argc > 2) {
    fprintf(2, "Too many arguments\n");
    exit(1);
  }

  i = atoi(argv[1]);
  sleep(i);

  exit(0);
}
