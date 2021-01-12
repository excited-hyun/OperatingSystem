#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

void syscall_init (void);

void exit(int status);
int fibonacci(int num);
int max_of_four_int(int num1, int num2, int num3, int num4);

#endif /* userprog/syscall.h */
