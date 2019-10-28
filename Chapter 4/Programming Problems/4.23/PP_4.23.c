#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int *SIEVE;
int INPUT = 0;

void printArr (int *arr, int n) {
	for (int i = 0; i < n; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
}

int nonZeroes (int *arr, int n) {
	int nonZero = 0;
	for (int i = 0; i < n; i++) {
		if(arr[i] != 0) {
			++nonZero;
		}
	}

	return nonZero;
}

void fillSieve () {
	for (int i = 0; i < INPUT; i++) {
		SIEVE[i] = i;
	}
}

void *printPrimes (void *param) {
	fillSieve();
	for (int i = 2; i*i <= INPUT; i++) {
		if(SIEVE[i] == 0) {
			continue;
		}
		for (int j = 2; j <= (INPUT-1) / i; j++) {
			SIEVE[j*i] = 0;
		}
	}
	
	SIEVE = SIEVE + 2;
	INPUT = INPUT - 2;
	int primeLen = 0;
	for (int i = 0; i < INPUT; i++) {
		int temp = 0;
		if(SIEVE[i]) {
			temp = SIEVE[primeLen];
			SIEVE[primeLen] = SIEVE[i];
			SIEVE[i] = temp;
			++primeLen;
		}
	}
	
	SIEVE = realloc(SIEVE - 2, (primeLen+2)*sizeof(int));
	SIEVE = SIEVE + 2;
	printf("Total number of primes in sequence: %d\n", primeLen);
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
	
	SIEVE = (int*)malloc((INPUT)*sizeof(int));
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
	free(SIEVE);

	return 0;
}

