#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

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
  char **argv;
  int fds[NOFILE];

  if(argstr(0, &path) < 0) // arg 0 = path string
    return -1;

  if(argptr(1, (char**)&argv, sizeof(argv) < 0)) // arg 1 = argv pointer
    return -1;
  
  if(argptr(2, (char**)&fds, NOFILE * sizeof(int)) < 0) // arg 2 = fds array, 16 ints
    return -1;

  return pcreate(path, argv, fds);
  
}
