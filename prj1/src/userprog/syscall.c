#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "lib/user/syscall.h"

void check_addr_valid(void *esp);
void check_all_valid(int cnt, void *esp);

static void syscall_handler (struct intr_frame *);
void exit(int status);
void halt();
pid_t exec(const char* cmd_line);
int wait(pid_t pid);
int read(int fd, void* buffer, unsigned size);
int write(int fd, const void* buffer, unsigned size);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}


void check_addr_valid(void *esp){
	if(!is_user_vaddr(esp))
		exit(-1);
}

void check_all_valid(int cnt, void *esp){
	for(int i=1; i<=cnt; i++){
		check_addr_valid((void*)((unsigned*)esp+i));
	}
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{

	void *esp_type;
	int type;
	void *args[4];

	esp_type = f -> esp;
	type = *(uint32_t*)esp_type;

	//printf("\n\n");

  
//	hex_dump(esp_type, esp_type, 300, 1);
  //hex_dump(*esp, *esp, 300, 1);
	
	if (type == SYS_FIBONACCI){

		check_all_valid(1, esp_type);
		args[0] = (void*)((unsigned*)esp_type+1);
		//args[1] = (void*)((unsigned*)esp_type+2);
		//args[2] = (void*)((unsigned*)esp_type+3);
		f -> eax = fibonacci((int)*(uint32_t*)args[0]);

		//printf("%d\n", f->eax);
		//f->eax = fibonacci();
	}
	else if (type == SYS_MAX_OF_FOUR_INT){
		//f->eax = max_of_four_int();
		check_all_valid(4, esp_type);
		args[0] = (void*)((unsigned*)esp_type+1);
		args[1] = (void*)((unsigned*)esp_type+2);
		args[2] = (void*)((unsigned*)esp_type+3);
		args[3] = (void*)((unsigned*)esp_type+4);
		f -> eax = max_of_four_int((int)*(uint32_t*)args[0], (int)*(uint32_t*)args[1], (int)*(uint32_t *)args[2], (int)*(uint32_t *)args[3]);

		//printf("%d\n", f->eax);
	}
	else if (type == SYS_HALT){
		halt();
	}
	else if (type == SYS_EXIT){
		check_addr_valid(f->esp + sizeof(uint32_t));
		exit(*(uint32_t*)(f->esp + sizeof(uint32_t)));
		
	}
	else if (type == SYS_EXEC){
		check_addr_valid(f->esp + sizeof(uint32_t));
		f -> eax = exec((char*)*(uint32_t*)(esp_type + sizeof(uint32_t)));
	}
	else if (type == SYS_WAIT){
		check_addr_valid(f->esp + sizeof(uint32_t));
		f -> eax = wait((int)*(uint32_t*)(f->esp + sizeof(uint32_t)));

	}
	else if (type == SYS_CREATE){

	}
	else if (type == SYS_REMOVE){

	}
	else if (type == SYS_OPEN){

	}
	else if (type == SYS_FILESIZE){

	}
	else if (type == SYS_READ){
		check_all_valid(3, esp_type);
		args[0] = (void*)((unsigned*)f->esp+1);
		args[1] = (void*)((unsigned*)f->esp+2);
		args[2] = (void*)((unsigned*)f->esp+3);
		f -> eax = read((int)*(uint32_t*)args[0], (void*)*(uint32_t*)args[1], (unsigned)*(uint32_t *)args[2]);

	}
	else if (type == SYS_WRITE){
		//printf("write\n");
		check_all_valid(3, f->esp);
		args[0] = (void*)((unsigned*)f->esp+1);
		args[1] = (void*)((unsigned*)f->esp+2);
		args[2] = (void*)((unsigned*)f->esp+3);
		f -> eax = write((int)*(uint32_t*)args[0], (void*)*(uint32_t*)args[1], (unsigned)*(uint32_t *)args[2]);
		//printf("%d\n", f->eax);
		//f->eax = write((int) * (uint32_t*)(f->esp + (5 * sizeof(uint32_t))), (void*)*(uint32_t*)(f->esp + (6 * sizeof(uint32_t))), (unsigned) * ((uint32_t*)(f->esp + (7 * sizeof(uint32_t)))));

	}
	else if (type == SYS_SEEK){

	}
	else if (type == SYS_TELL){

	}
	else if (type == SYS_CLOSE){

	}


	else if (type == SYS_MMAP){

	}
	else if (type == SYS_MUNMAP){

	}
  	//printf ("system call!\n");
  	//thread_exit ();
}

void halt(){
	shutdown_power_off();
}

void exit(int status){
	struct thread *current;
	current = thread_current();
	current -> exit_status = status;
	current -> parent -> child_exit_status = status;
//	printf("HI!!!!!!!!!\n");

	printf("%s: exit(%d)\n", current -> name, current -> exit_status);

	thread_exit();
}

pid_t exec(const char* cmd_line){
	return process_execute(cmd_line);
}

int wait(pid_t pid){
	return process_wait(pid);
}

int read(int fd, void* buffer, unsigned size){

	if(fd == 0){
		for(int i=0; i<size; i++){
			*(char*)buffer = input_getc();
			//printf("%c", *(char*)buffer);
			buffer++;
		}
	}
	return size;
}
int write(int fd, const void* buffer, unsigned size){
	
	//printf("!%d\n", fd);
	if(fd == 1){
		//printf("writng!%d\n", size);
		putbuf((char*)buffer, size);
		//printf("%d %d\n",fd, size);
		return size;

	}
	return -1;
}

int fibonacci(int num){
	int n0=1, n1=1, result = 0;

	if (num ==0){
		result = 0;
	}
	else if (num == 1|| num == 2){
		result = 1;
	}
	else{
		for(int i=3; i<= num; i++){
			result = n0 + n1;
			n0 = n1;
			n1 = result;
		}
	}

	return result;

}

int max_of_four_int(int num1, int num2, int num3, int num4){
	//printf("%d %d %d %d\n", num1, num2, num3, num4);
	int max = num1;
	if(num2 > max)
		max = num2;
	if(num3 > max)
		max = num3;
	if(num4 > max)
		max = num4;

	return max;
}

