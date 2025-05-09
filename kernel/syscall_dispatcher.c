#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SYSCALLS 256
#define MAX_SYSCALLS_NAME 64

#define KERNEL_MODE 1
#define USER_MODE 0

int current_mode = USER_MODE;


typedef long (*syscall_fn_t)(long arg1, long arg2, long arg3, long arg4, long arg5, long arg6);

typedef struct {

	char name[MAX_SYSCALLS_NAME];
	syscall_fn_t fn;
	int num_args;
	int implemented;


} syscall_entry_t;

syscall_entry_t syscall_table[MAX_SYSCALLS];
int next_syscall_id = 0;

void enter_kernel_mode(){
	printf("[KERNEL] Switching from user mode to kernel mode\n");
	current_mode = KERNEL_MODE;
}


void exit_kernel_mode(){
	printf("[KERNEL] Switching from kernel mode to user mode\n");
	current_mode = USER_MODE;
}


int register_syscall(const char* name, syscall_fn_t fn, int num_args){
	if(next_syscall_id >= MAX_SYSCALLS){
		printf("[KERNEL] Erro: System call table full\n");
		return -1;
	}

	if(strlen(name) >= MAX_SYSCALLS_NAME)
	{
		printf("[KERNEL] Error: System call name too long\n");
	}

	int syscall_id = next_syscall_id++;
	strncpy(syscall_table[syscall_id].name, name, MAX_SYSCALLS_NAME);
	syscall_table[syscall_id].fn = fn;
	syscall_table[syscall_id].num_args = num_args;
	syscall_table[syscall_id].implemented = 1;

	printf("[KERNEL] Registered system call '%s' with ID %d\n", name, syscall_id);
	return syscall_id;

}

long syscall_dispatcher(int syscall_num, long arg1, long arg2, long arg3, long arg4, long arg5, long arg6)
{
	enter_kernel_mode();

	printf("[KERNEL] Dispatching system call %d (%s)\n",
		syscall_num,
		syscall_table[syscall_num].name);

	long result = -1;

	if(syscall_num >= 0 && syscall_num < next_syscall_id && syscall_table[syscall_num].implemented)
	{
		result = syscall_table[syscall_num].fn(arg1, arg2, arg3, arg4, arg5, arg6);
		printf("[KERNEL] System call %s returned %ld\n",
			syscall_table[syscall_num].name,
			result);
	}
	else
	{
		printf("[KERNEL] Invalid system call number: %d\n", syscall_num);
		result = -1;
	}

	exit_kernel_mode();

	return result;
}


long syscall(int syscall_num, long arg1, long arg2, long arg3, long arg4, long arg5, long arg6)
{
	if(current_mode == KERNEL_MODE)
	{
		printf("[USER] Error: Already in kernel mode\n");
		return -1;
	}

	printf("[USER] Making system call %d (%s)\n",
		syscall_num,
		syscall_table[syscall_num].name);

	return syscall_dispatcher(syscall_num, arg1, arg2, arg3, arg4, arg5, arg6);
}

long sys_read(long fd, long buf_ptr, long count, long unused1, long unused2, long unused3)
{
	printf("[KERNEL] sys_read(fd=%ld, buf=%ld, count=%ld)\n", fd, buf_ptr, count);

	return count;
}


long sys_write(long fd, long buf_ptr, long count, long unused1, long unused2, long unused3)
{
	printf("[KERNEL] sys_write(fd=%ld, buf=%ld, count=%ld)\n", fd, buf_ptr, count);
	return count;
}


long sys_fork(long unused1, long unused2, long unused3, long unused4, long unused5, long unused6)
{
	printf("[KERNEL] sys_fork()\n");

	static int next_pid = 1000;
	return next_pid++;
}


long sys_exit(long exit_code, long unused1, long unused2, long unused3, long unused4, long unused5)
{
	printf("[KERNEL] sys_exit(code%ld)\n", exit_code);

	return 0;
}


int main()
{
	memset(syscall_table, 0, sizeof(syscall_table));

	int syscall_read = register_syscall("sys_read", sys_read, 3);
	int syscall_write = register_syscall("sys_write", sys_write, 3);
	int syscall_fork = register_syscall("sys_fork", sys_fork, 0);
	int syscall_exit = register_syscall("sys_exit", sys_exit, 1);

	long b_read = syscall(syscall_read, 0, 0x1000, 128, 0, 0, 0);
	printf("[USER] Read %ld bytes from stdin\n\n", b_read);

	long b_written = syscall(syscall_write, 1, 0x2000, 64, 0, 0, 0);
	printf("[USER] Wrote %ld bytes to stdout\n\n", b_written);

	long child_pid = syscall(syscall_fork, 0, 0, 0, 0, 0, 0);
	printf("[USER] Forked child process with PID %ld\n\n", child_pid);

	long exit_status = syscall(syscall_exit, 0, 0, 0, 0, 0, 0);
	printf("[USER] Process exited with status %ld\n\n", exit_status);

	long invalid_result = syscall(99, 0, 0, 0, 0, 0, 0);
	printf("[USER] Invalid syscall returned %ld\n", invalid_result);


	return 0;

}

























