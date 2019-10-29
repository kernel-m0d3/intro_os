#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int MIN_INT = 0x80000000;
int MAX_INT = 0x7fffffff;

int arr1[10] = {2,2,3,7,89,56,45,12,46,73};
int arr2[10] = {0};

struct data {
	int *array;
	unsigned int length;
};

void printArr (int *arr, int n) {
	for (int i = 0; i < n; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
}

void *sort (void *param) {
	struct data *passedData = (struct data *)param;
	
	for (int i = passedData->length -1; i > -1 ; --i) {
	
		int max = MIN_INT;
		int maxIndex = 0;
		
		for (int j = 0; j < i + 1; j++) {
			if (passedData->array[j] > max) {
				max = passedData->array[j];
				maxIndex = j;
			}
		}
		
		max = passedData->array[maxIndex];
		passedData->array[maxIndex] = passedData->array[i];
		passedData->array[i] = max;
	}
	
	pthread_exit(0);
}

void *merge () {
	pthread_t tid[2];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	int status;
	struct data *passedData[2];
	
	int arrlen = (sizeof arr1)/sizeof arr1[0];
	
	printArr(arr1, arrlen);
	printf("\nSorting...\n");
	
	for (int i = 0; i < 2; i++) {
		passedData[i] = (struct data *) malloc(sizeof(struct data));
		passedData[i]->array = arr1 + (arrlen/2)*i;
		passedData[i]->length = arrlen/2;
		status = pthread_create(&tid[i], &attr, *sort, (void*)passedData[i]);
		if (status < 0) {
			perror("pthread_create");
			exit(EXIT_FAILURE);
		}
	}
	for (int i = 0; i < 2; i++) {
		pthread_join(tid[i], NULL);
	}
	for (int i = 0; i < 2; i++) {
		free(passedData[i]);
	}
	
	
	int *secondHalf = arr1 + arrlen / 2;
	int *firstHalf = arr1;
	for (int i = 0; i < arrlen; i++) {
		if(*firstHalf < *secondHalf) {
			arr2[i] = *firstHalf;
			firstHalf++;
			if (firstHalf == arr1 + arrlen / 2) {
				firstHalf = &MAX_INT;
			}
		} else {
			arr2[i] = *secondHalf;
			secondHalf++;
			if (secondHalf == arr1 + arrlen) {
				secondHalf = &MAX_INT;
			}
		}
	}

	printArr(arr2, arrlen);
	
	pthread_exit(0);
}

void generateRandom () {
	srand(time(0));
	for (int i = 0; i < 10; i++) {
		arr1[i] = rand() % 100;
	}
}

void checkIfSorted () {
	for (int i = 1; i < 10; i++) {
		if(arr2[i-1] > arr2[i]) {
			printf("Array not sorted...\n");
			return;
		}
	}
	printf("\nCongratulations! Array sorted.\n");
}

int main (int argc, char **argv) {
	printf("Generating 10 random numbers...\n");
	generateRandom();
	
	pthread_t tid;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	int status;
	
	status = pthread_create(&tid, &attr, merge, NULL);
	if (status < 0) {
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}
	pthread_join(tid,NULL);
	checkIfSorted();

	return EXIT_SUCCESS;
}
