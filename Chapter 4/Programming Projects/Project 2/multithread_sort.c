/*

Multithreaded Sorting Application

*/



#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int MIN_INT = 0x80000000;
int MAX_INT = 0x7fffffff;

int arr1[] = {2,2,3,7,89,56,45,12,46,73,34};
const int arrLength = (sizeof arr1)/sizeof arr1[0];
int *arr2;

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
	
	printArr(arr1, arrLength);
	printf("\nSorting...\n");
	
	for (int i = 0; i < 2; i++) {
		passedData[i] = (struct data *) malloc(sizeof(struct data));
		passedData[i]->array = arr1 + (arrLength/2)*i;
		passedData[i]->length = abs(arrLength*i - arrLength/2);
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
	
	
	int *secondHalf = arr1 + arrLength / 2;
	int *firstHalf = arr1;
	for (int i = 0; i < arrLength; i++) {
		if(*firstHalf < *secondHalf) {
			arr2[i] = *firstHalf;
			firstHalf++;
			if (firstHalf == arr1 + arrLength / 2) {
				firstHalf = &MAX_INT;
			}
		} else {
			arr2[i] = *secondHalf;
			secondHalf++;
			if (secondHalf == arr1 + arrLength) {
				secondHalf = &MAX_INT;
			}
		}
	}

	printArr(arr2, arrLength);
	
	pthread_exit(0);
}

void generateRandom (int length) {
	srand(time(0));
	for (int i = 0; i < length; i++) {
		arr1[i] = rand() % 100;
	}
}

void checkIfSorted (int *array, int length) {
	for (int i = 1; i < length; i++) {
		if(array[i-1] > array[i]) {
			printf("Array not sorted...\n");
			return;
		}
	}
	printf("\nCongratulations! Array sorted.\n");
}

int main (int argc, char **argv) {
	printf("Generating %d random numbers...\n", arrLength);
	generateRandom(arrLength);
	
	arr2 = (int *)malloc((sizeof (int))*arrLength);
	
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
	checkIfSorted(arr2, arrLength);

	return EXIT_SUCCESS;
}
