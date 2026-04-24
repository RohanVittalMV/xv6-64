# CS 5460 Homework 4 - Part 1

## Explanation of freerange() dump 

### Breakpoint hit: start of freerange()

```
rsp = 0x8000011eb88
rbp = 0x8000011eb98
rip = 0x80000102c88 <freerange+10>
```
Call chain from `bt`: 
```
#0  freerange(vstart=0x8000011ebf0, vend=0x8000e000000)  kalloc.c:50
#1  kinit1(vstart=0x8000011ebf0, vend=0x8000e000000)     kalloc.c:36
#2  main()                                                main.c:22
```

### What happens before the breakpoint
4 8-byte values have been pushed onto the stack, and `rsp` has moved down by 32 from its value before `call()` in `kinit1()`.

```asm
push   %rbp        ; # save kinit1's frame pointer
mov    %rsp,%rbp   ; # establish freerange's frame pointer  <- rbp = 0x8000011eb98
push   %r12        ; # save callee-saved register r12
push   %rbx        ; # save callee-saved register rbx       <- rsp = 0x8000011eb88
mov    %rsi,%r12   ; # r12 = vend (freerange's 2nd arg)
; # breakpoint hits here
```

### Stack dump and explanation 
```
0x8000011eb88 <stack+3992>:  0x0000080000118ac0   0x0000000000000000
0x8000011eb98 <stack+4008>:  0x000008000011ebc8   0x0000080000102d07
0x8000011eba8 <stack+4024>:  0x0000000000000000   0x000008000e000000
0x8000011ebb8 <stack+4040>:  0x0000000000000000   0x0000000000000000
0x8000011ebc8 <stack+4056>:  0x000008000011ebe8   0x0000080000103a09
0x8000011ebd8 <stack+4072>:  0x0000000000000000   0x00000000000100b0
```

- `freerange()` stack frame (`rsp` to `rbp + 8`):
    - These 4 values are results of `freerange`'s prologue. 
    - `rbx` is callee saved so `freerange` saves it at the start of the function. `r12` is also callee saved, and `freerange` saves it as well. `r12` has value zero, showing `kinit1()` was not using it for anything at the time of the call. 
    - `rbp` is the frame pointer for `freerange`, and it points to the saved `rbp` of `kinit1()`.
    - `rbp + 8` is the return address to `kinit1()`, which is the instruction immediately after the `call` to `freerange()`.

### `kinit1()` stack frame
- everything from `0x8000011eba8` onward is not part of `kinit1()`'s stack frame. 
- from 0x8000011eba8 -> 0x0000000000000000 — `kinit1` local / alignment padding. The compiler inserted this to maintain 16 byte alignment.
- from 0x8000011ebb0 → 0x000008000e000000 — spilled vend argument. `kinit1` receives `vend` = 0x8000e000000 in `rsi`. Before calling `initlock` the compiler spilled this to the stack frame so that it could use `rsi` for the call to `initlock`. 
- from 0x8000011ebb8 → 0x0000000000000000 — `kinit1` local / alignment padding. Inserted to maintain 16 byte alignment.
- from 0x8000011ebc8 (`kinit1`'s `rbp`) → 0x000008000011ebe8 — saved frame pointer (main's `rbp`) `kinit1`'s own prologue saved main's `rbp` here. 
    - This continues the frame pointer chain
    - freerange's rbp → `kinit1`'s `rbp` → main's `rbp`. 
    - The value 0x8000011ebe8 is main's frame pointer.

### `main()` stack frame

- from 0x8000011ebd8 → 0x0000000000000000 — main local / alignment padding. This is within main's frame, above the return address from main.
- 0x8000011ebe0 → 0x00000000000100b0 — main local variable or spilled argument
    - The value 0x100b0 is a low physical address, so this is likely a pointer to some data structure in the kernel's physical memory.


