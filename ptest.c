#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]) {
  int fds[16];
  char *args[] = {"ls", 0};

  for (int i = 0; i < 16; i++)
    fds[i] = -1;

  int pid = pcreate("/ls", args, fds);
  printf(1, "pcreate returned: %d\n", pid);

  int wpid = wait();
  printf(1, "wait returned: %d\n", wpid);

//   pcreate("/doesnotexist", args, fds);

  //   int fd = open("/README", 0);
  //   if (fd < 0) {
  //     printf(1, "open failed\n");
  //     exit();
  //   }

  //   fds[0] = fd; // child fd 0 <- parent fd X
  //   fds[1] = 1;  // child fd 1 <- parent fd 1 (stdout)

  //   int pid = pcreate("/cat", args, fds);
  //   printf(1, "pcreate returned pid: %d\n", pid);
  exit();
}