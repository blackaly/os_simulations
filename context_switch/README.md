# Context Switch in xv6

This is the context switch assembly code from xv6. In C. It's declared as
```c
void swtch(struct context** old, struct context* new);
```

**%esp**: stack pointer


When you switch from one process to another, the OS must:-
1. Save all important CPU registers for the current process.
2. Load all saved registers for the new process.
Otherwise, when the new process resumes, it won't know where it left off.



[Process A running]
    |
save A's registers ➔ load B's registers ➔ jump to B's code
    |
[Process B running]
