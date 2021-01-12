#include <stdio.h>
#include <stdlib.h>
#include <syscall.h>

int main(int argc, char* argv[]){

	int fibo = 0;
	int max = 0;
	int num[4];

	for(int i=0; i<4; i++){
		num[i] = atoi(argv[i+1]);
	}

	//printf("Hi\n");
	fibo = fibonacci(num[0]);
	//printf("%d\n", fibo);
	max = max_of_four_int(num[0], num[1], num[2], num[3]);

	printf("%d %d\n", fibo, max);

	return EXIT_SUCCESS;
}
