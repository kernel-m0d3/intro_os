#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define ARR_SIZE 255
#define NUM_THREADS 3

int INPUT[ARR_SIZE] = {0};
int INPUT_LEN = 0;
int MAX = 0;
int MIN = 0;
float AVE = 0;

int MAX_INT = 0x7fffffff;
int MIN_INT = 0x80000000;


void *average (void *param) {
	float ave = 0;
	for (int i = 0; i < INPUT_LEN; i++) {
		ave += INPUT[i];
	}
	ave /= INPUT_LEN;
	AVE = ave;
	pthread_exit(0);
}

void *maximum (void *param) {
	int max = MIN_INT;
	for (int i = 0; i < INPUT_LEN; i++) {
		max = max > INPUT[i] ? max : INPUT[i];
	}
	MAX = max;
	pthread_exit(0);
}

void *minimum (void *param) {
	int min = MAX_INT;
	for (int i = 0; i < INPUT_LEN; i++) {
		min = INPUT[i] < min ? INPUT[i] : min;
	}
	MIN = min;
	pthread_exit(0);
}

void parseInput (char **argv, int len) {
	for (int i = 0; i < len; i++) {
		INPUT[i] = atoi(argv[i+1]);
	}
	return;
}

void printArr () {
	for (int i = 0; i < INPUT_LEN | INPUT[i] != 0; i++) {
		printf("%d ", INPUT[i]);
	}
}


int main (int argc, char **argv) {

	if (argc < 2) {
		return 0;
	}
	INPUT_LEN = argc -1;
	parseInput(argv, INPUT_LEN);
	
	
	pthread_t tid[NUM_THREADS];
	pthread_attr_t attr;
	
	pthread_attr_init(&attr);
	int status;
	
	void *(*func_arr[])(void *) = {average, maximum, minimum}; 
	
	
	for (int i = 0; i < NUM_THREADS; i++) {
		status = pthread_create(&tid[i], &attr, *func_arr[i], NULL);
		if (status < 0) {
			perror("pthread_create");
			exit(EXIT_FAILURE);
		}
	}
	
	for (int i = 0; i < NUM_THREADS; i++) {
		pthread_join(tid[i], NULL);
	}
	
	printf("The average value is : %.2f\n", AVE);
	printf("The minimum value is : %d\n", MIN);
	printf("The maximum value is : %d\n", MAX);

	return 0;
}
