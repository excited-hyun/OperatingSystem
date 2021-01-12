#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include <string.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "threads/synch.h"
#include "lib/user/syscall.h"
#include "devices/shutdown.h"
#include "devices/input.h"
#include "filesys/off_t.h"
#include "filesys/filesys.h"
#include "filesys/file.h"

struct lock file_lock;

void check_addr_valid(void *esp);
void check_all_valid(int cnt, void *esp);

static void syscall_handler (struct intr_frame *);
//void exit(int status);
void halt();
pid_t exec(const char* cmd_line);
int wait(pid_t pid);

int read(int fd, void* buffer, unsigned size);
int write(int fd, const void* buffer, unsigned size);

int sum_of_four_integers(int n1, int n2, int n3, int n4);
int fibonacci(int n);

bool create(const char* file, unsigned init_size);
bool remove(const char* file);
int open(const char* file);
int filesize(int fd);
void seek (int fd, unsigned position);
unsigned tell (int fd);
void close (int fd);


void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
  lock_init(&file_lock);
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


	/*------------------------------prj2--------------------------*/
	
	else if (type == SYS_CREATE){
		check_all_valid(2, esp_type);
		args[0] = (void*)((unsigned*)esp_type+1);
		args[1] = (void*)((unsigned*)esp_type+2);
		f->eax = create((const char*)*(uint32_t*)args[0], (unsigned)*(uint32_t*)args[1]);

	}
	else if (type == SYS_REMOVE){

		check_all_valid(1, esp_type);
		args[0] = (void*)((unsigned*)esp_type+1);
		f->eax = remove((const char*)*(uint32_t*)args[0]);

	}
	else if (type == SYS_OPEN){
		check_all_valid(1, esp_type);
		args[0] = (void*)((unsigned*)esp_type+1);
		f->eax = open((const char*)*(uint32_t*)args[0]);
	}
	else if (type == SYS_FILESIZE){
		check_all_valid(1, esp_type);
		args[0] = (void*)((unsigned*)esp_type+1);
		f->eax = filesize((int)*(uint32_t*)args[0]);
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
		check_all_valid(2, esp_type);
		args[0] = (void*)((unsigned*)f->esp+1);
		args[1] = (void*)((unsigned*)f->esp+2);
		seek((int)*(uint32_t*)args[0], (unsigned)*(uint32_t*)args[1]);
	}
	else if (type == SYS_TELL){

		check_all_valid(1, esp_type);
		args[0] = (void*)((unsigned*)f->esp+1);
		f->eax = tell((int)*(uint32_t*)args[0]);
	}
	else if (type == SYS_CLOSE){
		check_all_valid(1, esp_type);
		args[0] = (void*)((unsigned*)f->esp+1);
		close((int)*(uint32_t*)args[0]);

	}

/*-----------------------------------prj3-------------------------------*/
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
	//current -> parent -> child_exit_status = status;
//	printf("HI!!!!!!!!!\n");
	for(int i=3;i<131;i++){
		if(current->file_des[i] != NULL) close(i);
	}

	printf("%s: exit(%d)\n", current -> name, status);

	thread_exit();
}

pid_t exec(const char* cmd_line){
	return process_execute(cmd_line);
}

int wait(pid_t pid){
	return process_wait(pid);
}

int read(int fd, void* buffer, unsigned size){

	int result, count = 0;
	check_addr_valid(buffer);
	lock_acquire(&file_lock);

	//stdin
	if(fd == 0){
		check_addr_valid(buffer);
		count = 0;
		for(int i=0; i<size; i++){
			*(char*)buffer = input_getc();
			//printf("%c", *(char*)buffer);
			buffer++;
			count++;
		}
		lock_release(&file_lock);
		return size;
	}

	//file read
	else if(fd >= 2){
		//lock_acquire(&file_lock);
		struct thread* current = thread_current();
		check_addr_valid(buffer);
		result = file_read(current->file_des[fd], buffer, size);
		lock_release(&file_lock);
		return result;
	}

	lock_release(&file_lock);
	return -1;
}
int write(int fd, const void* buffer, unsigned size){
	
	int result;
	//printf("!%d\n", fd);

	check_addr_valid(buffer);
	lock_acquire(&file_lock);
	//stdout
	if(fd == 1){
		//printf("writng!%d\n", size);
		putbuf((char*)buffer, size);
		//printf("%d %d\n",fd, size);
		lock_release(&file_lock);
		return size;

	}

	else if(fd >= 3){
		//lock_acquire(&file_lock);
		struct thread* current = thread_current();
		if(current->file_des[fd]==NULL){
			lock_release(&file_lock);
			return -1;
		}
		result = file_write(current->file_des[fd], buffer, size);
		lock_release(&file_lock);
		return result;

	}
	
	lock_release(&file_lock);
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

bool create(const char* file, unsigned init_size){

	bool result;
	if(file == NULL)
		exit(-1);
	lock_acquire(&file_lock);
	result = filesys_create(file, init_size);
	lock_release(&file_lock);

	return result;

}
bool remove(const char* file){

	bool result;
	if(file == NULL)
		exit(-1);
//	lock_acquire(&file_lock);
	result = filesys_remove(file);
//	lock_release(&file_lock);

	return result;
}

int open(const char* file){

//	printf("AAAAAA %s\n", file);
	if(file == NULL)
		exit( -1);

	//lock_acquire(&file_lock);

	check_addr_valid(file);
	int fd;
	struct thread* current = thread_current();
	struct file* ofp;
	int result = -1;

	if(current->file_cnt>131){
		//lock_release(&file_lock);
		return -1;
	}

	lock_acquire(&file_lock);
		
	ofp = filesys_open(file);

	if(ofp == NULL){
		lock_release(&file_lock);
		return -1;

	}
	else{
		
		//lock_acquire(&file_lock);
		if(strcmp(current->name, file)==0)
			file_deny_write(ofp);

		fd = 3 + current->file_cnt++;
		current->file_des[fd] = ofp;
		lock_release(&file_lock);
		return fd;
	}
}
int filesize(int fd){
	
	struct thread* current = thread_current();
	if(current->file_des[fd] == NULL)
		return -1;
	if (fd < 3)
		return -1;

	return file_length(current->file_des[fd]);
}
void seek (int fd, unsigned position){

	struct thread* current = thread_current();
	if(current->file_des[fd] == NULL)
		return ;
		//exit(-1);
	if (fd < 3)
		return ;
	/*--------------*/
	return file_seek(current->file_des[fd], position);

}
unsigned tell (int fd){

	struct thread* current = thread_current();
	if(current->file_des[fd] == NULL)
		return -1;
	if (fd < 3)
		return -1;

	return file_tell(current -> file_des[fd]);
}
void close (int fd){

	struct thread* current = thread_current();
	if(current->file_des[fd] == NULL)
		return ;
		//	exit( -1);
	if (fd < 3)
		return ;

	

	file_close(current->file_des[fd]);
	current->file_des[fd] = NULL;


}

