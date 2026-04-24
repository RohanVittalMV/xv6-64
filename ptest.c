#include "stat.h"
#include "types.h"
#include "user.h"

int main(int argc, char *argv[]) {
  int fds[16];
  char *args[] = {"ls", 0};

  for (int i = 0; i < 16; i++)
    fds[i] = -1;

  int pid = pcreate("/ls", args, fds);
  printf(1, "pcreate returned: %d\n", pid);
  exit();
}