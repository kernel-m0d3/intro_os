/*

Prime Number Application

*/

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

unsigned long long int *SIEVE;
unsigned int INPUT = 0;

void printArr (unsigned long long int *arr, unsigned int n) {
	for (int i = 0; i < n; i++) {
		printf("%llu ", arr[i]);
	}
	printf("\n");
}

int nonZeroes (unsigned long long int *arr, unsigned int n) {
	int nonZero = 0;
	for (int i = 0; i < n; i++) {
		if(arr[i] != 0) {
			++nonZero;
		}
	}

	return nonZero;
}

void fillSieve () {
	for (unsigned long long int i = 0; i < INPUT; i++) {
		SIEVE[i] = i;
	}
}

void *printPrimes (void *param) {
	fillSieve();
	for (unsigned long long int i = 2; i*i <= INPUT; i++) {
		if(SIEVE[i] == 0) {
			continue;
		}
		for (unsigned long long int j = 2; j <= (INPUT-1) / i; j++) {
			SIEVE[j*i] = 0;
		}
	}
	
	SIEVE = SIEVE + 2;
	INPUT = INPUT - 2;
	unsigned int primeLen = 0;
	for (int i = 0; i < INPUT; i++) {
		int temp = 0;
		if(SIEVE[i]) {
			temp = SIEVE[primeLen];
			SIEVE[primeLen] = SIEVE[i];
			SIEVE[i] = temp;
			++primeLen;
		}
	}
	
	SIEVE = realloc(SIEVE - 2, (primeLen+2)*sizeof(unsigned long long int));
	SIEVE = SIEVE + 2;
	printf("Total number of primes in sequence: %u\n", primeLen);
	printArr(SIEVE, primeLen);
	SIEVE = SIEVE - 2;
	INPUT = primeLen;
	
	pthread_exit(0);
}

int main (int argc, char **argv) {
	if (argc < 2 || argv[1] < 0) {
		printf("Enter a positive integer...\n");
		return 0;
	}	
	INPUT = atoi(argv[1]);
	INPUT++;
	
	SIEVE = (unsigned long long int*)malloc((INPUT)*sizeof(unsigned long long int));
	if(errno == ENOMEM) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	
	pthread_t tid;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	int status;
	
	status = pthread_create(&tid, &attr, printPrimes, NULL);
	if (status < 0) {
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}
	
	pthread_join(tid, NULL);
	printf("Successfully printed %u prime numbers.\n", INPUT);
	free(SIEVE);

	return 0;
}

