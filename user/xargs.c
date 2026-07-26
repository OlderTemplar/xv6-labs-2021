#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define ARGLENGTH 32

int
main(int argc, char *argv[])
{
  char buf[512], *c = malloc(ARGLENGTH), arg[33],
                 **new_argv = malloc((MAXARG + 1) * sizeof(char *));
  int i = 0, pid, new_argc=0;
  new_argv[MAXARG] = 0;

  //初始化new_argv
  for (int j = 0; j < MAXARG; j++) {
    new_argv[j] = malloc(ARGLENGTH);
  }
  while ((new_argv[i] = argv[i + 1]) != 0) {
    i++;
    new_argc++;
  }


  if (argc < 2) {
    // 没有提供要执行的命令，直接退出或报错
    fprintf(2, "xargs lacks command\n");
    exit(1);
  }

  // read arguments into buf
   i = 0; // used for buf
   while (read(0, c, 1)) {
      buf[i++] = *c;
     if (i == 512) {
        fprintf(2, "xargs: input is too long\n");
       exit(1);
     }
   }
   buf[i] = '\0';


   // inspect \n then fork() and exec()
   i = 0; // used for arg
   int k = new_argc; // used for new_argv
   for (int j = 0; j < strlen(buf); j++) {

     // store arg into new_argv
     if ((buf[j] == ' ') || (buf[j] == '\n')) {
       while (buf[j + 1] == ' ') {
         j++;
       }
       arg[i] = '\0';

       if (new_argc >= MAXARG) {
         fprintf(2, "xargs:too many arguments\n");
         exit(1);
       }

       if (new_argv[k] == 0)
         new_argv[k] = c;
       strcpy(new_argv[k++], arg);
       i = 0;

       if (buf[j] == ' ')
        continue;
     }

     if (buf[j] == '\n') {
       new_argv[k] = 0;
       pid = fork();
       if (pid != 0) {
         wait(0);
         k = new_argc;
         continue;
       } else {
         if (exec(new_argv[0], new_argv) < 0) {
           fprintf(2, "xargs exec %s failed", new_argv[0]);
           exit(1);
         }
       }
     }

     arg[i++] = buf[j];

   }

   //free new_argv
   for (int j = 0; j < MAXARG + 1; j++) {
     free(new_argv[j]);
   }
   free(new_argv);
   free(c);
   exit(0);
}
