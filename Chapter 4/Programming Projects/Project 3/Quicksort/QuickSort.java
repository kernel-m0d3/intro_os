/*

Quicksort Sorting Application

*/

import java.util.Arrays;
import java.util.concurrent.*;

public class QuickSort extends RecursiveAction {
	private int[] sourceArray;
	private int startIndex;
	private int endIndex;
	
	public QuickSort (int[] src, int start, int end) {
		sourceArray = src;
		startIndex = start;
		endIndex = end;
	}
	
	protected void sortDirectly (int start, int endIndex) {
		Arrays.sort(sourceArray, start, endIndex + 1);
	}
	
	protected int partition(int arr[], int low, int high) 
	{ 
		int pivot = arr[high]; 
		int i = (low-1);
		for(int j=low; j<high; j++) {
			if(arr[j] < pivot) { 
				i++;
				int temp = arr[i]; 
				arr[i] = arr[j]; 
				arr[j] = temp; 
			}
		}
		int temp = arr[i+1]; 
		arr[i+1] = arr[high]; 
		arr[high] = temp; 

		return i+1; 
	}
	
	protected static int sThreshold = 10;
	
	@Override
	protected void compute() {
		if((endIndex - startIndex + 1) < sThreshold) {
			sortDirectly(startIndex, endIndex);
			return;
		}
		
		int partIndex = partition(sourceArray, startIndex, endIndex);
		
		QuickSort leftTask = new QuickSort(sourceArray, startIndex, partIndex - 1);
		QuickSort rightTask = new QuickSort(sourceArray, partIndex + 1, endIndex);
		
		leftTask.fork();
		rightTask.fork();
		
		rightTask.join();
		leftTask.join();
	}
	
	static void printArray (int[] array, int length) {
		for (int i = 0; i < length; i++) {
			System.out.print(array[i] + " ");
		}
		System.out.print("\n");
	}

	public static void main(String[] args) {
		
		int[] inputArray = {5,43,54,24,36,37,23,12,8,7,5,3,5,6,9};
		
		printArray(inputArray, inputArray.length);
		
		ForkJoinPool pool = new ForkJoinPool();
		
		QuickSort task = new QuickSort(inputArray, 0, inputArray.length - 1);
		
		pool.invoke(task);
		
		printArray(inputArray, inputArray.length);

	}
}

