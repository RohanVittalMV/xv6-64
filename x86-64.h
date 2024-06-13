#include "x86.h"

struct segdesc;

static inline void
lgdt(struct segdesc *p, int size)
{
  volatile ushort pd[5];

  pd[0] = size-1;
  pd[1] = (uint64)p;
  pd[2] = (uint64)p >> 16;
  pd[3] = (uint64)p >> 32;
  pd[4] = (uint64)p >> 48;

  asm volatile("lgdt (%0)" : : "r" (pd));
}

struct gatedesc;

static inline void
lidt(struct gatedesc *p, int size)
{
  volatile ushort pd[5];

  pd[0] = size-1;
  pd[1] = (uint64)p;
  pd[2] = (uint64)p >> 16;
  pd[3] = (uint64)p >> 32;
  pd[4] = (uint64)p >> 48;

  asm volatile("lidt (%0)" : : "r" (pd));
}

static inline uint64
rcr2(void)
{
  uint64 val;
  asm volatile("movq %%cr2,%0" : "=r" (val));
  return val;
}

static inline void
lcr3(uint64 val)
{
  asm volatile("movq %0,%%cr3" : : "r" (val));
}

static inline uint64
readrflags(void)
{
    uint64 rflags;
    asm volatile("pushfq; popq %0" : "=r" (rflags));
    return rflags;
}