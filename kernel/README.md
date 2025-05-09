# syscall_dispatcher.c

- `MAX_SYSCALLS` limits our system call table size (real Linux has 300+ syscalls) and `MAX_SYSCALL_NAME ` defines the maximum length for syscall names. You can also conclude that `KERNEL_MODE` and `USER_MODE` help simulate the separation between privilege levels.

- `syscall_fn_t` defines each syscall table entry with:
	- Name for debugging.
	- Function pointer to the implementation.
	- Number of arguments it takes.
	- Flag indicating it's implemented.


## Mode Switching Functions

```c
void enter_kernel_mode(){
	printf("[KERNEL] Switching from user mode to kernel mode\n");
	current_mode = KERNEL_MODE;
}


void exit_kernel_mode(){
	printf("[KERNEL] Switching from kernel mode to user mode\n");
	current_mode = USER_MODE;
}

```
These functions simulate the privilege level transition that occurs when:
- A process makes a syscall (user -> kernel)
- The kernel finishes handling the call (kernel -> user)


In a real kernel, this involves CPU privilege ring transitions (ring 3 -> ring 0).
![CPU privilege ring](https://lh6.googleusercontent.com/proxy/w7klCBMPb1ZTOTGEzHJsK6ynqXuVYAm4bOWC-xXya9p-0tBwB1BiCVSLpXc05x7srosdn3wLoW9eIFJz3StJT8XTyUKtWiI "CPU privilege ring transitions")




