#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void
seek(char *path, char *obj)
{
  char *p, *buf = (char*)malloc(512);
  int fd, n_size;
  struct dirent de;
  struct stat st;

  if ((fd = open(path, 0)) < 0) {
    fprintf(2, "find:cannot open %s\n", path);
    return;
  }

  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  if (st.type != T_DIR)
    fprintf(2, "given path %s is not a directory\n", path);

  strcpy(buf, path);
  p = buf + strlen(buf);
  *p++ = '/';
  while (read(fd, &de, sizeof(de)) == sizeof(de)) {
    if (de.inum == 0)
      continue;
    memmove(p, de.name, DIRSIZ);
    n_size = sizeof(de.name);
    p[n_size] = '\0';

    //查找目标文件
    if (!strcmp(de.name, obj))
      fprintf(1, "%s\n", buf);

    if (stat(buf, &st) < 0) {
      printf("find: cannot stat %s\n", buf);
      continue;
    }

    if (st.type == T_DIR) {
      //不要进入.和..递归
      if (!strcmp(de.name, ".") || !strcmp(de.name, ".."))
        continue;

      seek(buf, obj);
    }
  }

  close(fd);
  free(buf);
}


int
main(int argc, char* argv[])
{

  if (argc < 3) {
    fprintf(2, "lack of argument\n");
    exit(1);
  }

  if (argc > 3) {
    fprintf(2, "too many argument\n");
    exit(1);
  }

  seek(argv[1], argv[2]);
  exit(0);
}
