#include "types.h"
#include "date.h"
#include "defs.h"
#include "elf.h"
#include "memlayout.h"
#include "mmu.h"
#include "param.h"
#include "proc.h"
#include "x86.h"

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

uint64
sys_wait(void)
{
  return wait();
}

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_sbrk(void)
{
  int64 addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64 sys_pcreate(void){
  char *path;
  char *argv[MAXARG];
  int fds[NOFILE];
  int i;
  uint64 uargv = -1, uarg, ufds = -1;

  cprintf("sys_pcreate entry");

  cprintf("sys_pcreate: entered\n");

  if (argstr(0, &path) < 0) {
    cprintf("sys_pcreate: argstr failed\n");
    return -1;
  }
  cprintf("sys_pcreate: path=%s\n", path);

  if (argint64(1, (int64 *)&uargv) < 0) {
    cprintf("sys_pcreate: argint64 uargv failed\n");
    return -1;
  }
  cprintf("sys_pcreate: uargv=%p\n", uargv);

  if (argint64(2, (int64 *)&ufds) < 0) {
    cprintf("sys_pcreate: argint64 ufds failed\n");
    return -1;
  }
  cprintf("sys_pcreate: ufds=%p\n", ufds);

  // Copy argv strings out of user space (same pattern as sys_exec)
  memset(argv, 0, sizeof(argv));
  for (i = 0;; i++) {
    if (i >= NELEM(argv))
      return -1;
    uarg = ((uint64 *)uargv)[i];
    if (uarg == 0) {
      argv[i] = 0;
      break;
    }
    if (fetchstr(uarg, &argv[i]) < 0)
      return -1;
  }

  // Copy fds array out of user space
  for (i = 0; i < NOFILE; i++) {
    fds[i] = ((int *)ufds)[i];
  }

  return pcreate(path, argv, fds);
}

