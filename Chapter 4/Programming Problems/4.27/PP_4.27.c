#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

unsigned long long int *FIBONACCI;
unsigned int INPUT = 0;

void printArr (unsigned long long int *arr, unsigned int n) {
	for (unsigned int i = 0; i < n; i++) {
		printf("%llu ", arr[i]);
	}
	printf("\n");
}

void *generateFibonacciSequence (void *param) {
	FIBONACCI[0] = 0;
	FIBONACCI[1] = 1;
	for (unsigned int i = 2; i < INPUT; i++) {
		FIBONACCI[i] = FIBONACCI[i-1] + FIBONACCI[i-2];
		if(FIBONACCI[i] < 0) {
			FIBONACCI[i] = 0;
			INPUT = i;
			break;
		}
	}
	FIBONACCI = realloc(FIBONACCI, INPUT*sizeof(unsigned long long int));
	
	pthread_exit(0);
}

int main (int argc, char **argv) {
	if (argc < 2 || argv[1] < 0) {
		printf("Enter a positive integer...\n");
		return 0;
	}	
	INPUT = atoi(argv[1]);
	
	FIBONACCI = (unsigned long long int*)malloc((INPUT)*sizeof(unsigned long long int));
	if(errno == ENOMEM) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	
	pthread_t tid;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	int status;
	
	status = pthread_create(&tid, &attr, generateFibonacciSequence, NULL);
	if (status < 0) {
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}
	
	pthread_join(tid, NULL);
	printf("Printing %u fibonacci numbers...\n", INPUT);
	printArr(FIBONACCI, INPUT);
	printf("Printed %u fibonacci numbers.\n", INPUT);
	free(FIBONACCI);

	return 0;
}

