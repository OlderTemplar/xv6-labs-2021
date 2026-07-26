#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int n;
  n = uptime();
  fprintf(1, "%d\n", n);
}
