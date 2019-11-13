/*

Mergesort Sorting Application

*/

/*

Mergesort Sorting Application

*/

import java.util.Arrays;
import java.util.concurrent.*;

class MergeSort extends RecursiveAction {
	private int[] sourceArray;
	private int[] destArray;
	private int[] leftArray;
	private int[] rightArray;
	private int startIndex;
	private int endIndex;
	private int arrLength;
	private static int pass;
	private int selfpass;
	
	public MergeSort (int[] src, int start, int end, int[] dest) {
		sourceArray = src;
		startIndex = start;
		endIndex = end;
		arrLength = end - start + 1;
		destArray = dest;
		pass++;
		selfpass = pass;
		//System.out.println("Pass " + selfpass + ": ");
		//printElements(sourceArray, startIndex, endIndex);
	}
	
	protected void sortDirectly (int start, int end) {
		Arrays.sort(sourceArray, start, end + 1);
	}
	
	protected void merge (int half) {
		int leftTop = startIndex;
		int rightTop = half + 1;
		
		printArray(sourceArray);
		
		for (int i = startIndex; i <= endIndex; i++) {
			if(leftTop == (half + 1)) {
				System.out.println(i + "Taking re " + sourceArray[rightTop]);
				destArray[i] = sourceArray[rightTop];
				rightTop++;
			} else if(rightTop == (endIndex + 1)) {
				System.out.println(i + "Taking le " + sourceArray[leftTop]);
				destArray[i] = sourceArray[leftTop];
				leftTop++;
			} else if(sourceArray[leftTop] <= sourceArray[rightTop]) {
				System.out.println(i + "Taking l " + sourceArray[leftTop]);
				destArray[i] = sourceArray[leftTop];
				leftTop++;
			} else {
				System.out.println(i + "Taking r " + sourceArray[rightTop]);
				destArray[i] = sourceArray[rightTop];
				rightTop++;
			}
		}
	}
	
	protected static int sThreshold = 10;
	
	@Override
	protected void compute() {
		if(arrLength <= sThreshold) {
			sortDirectly(startIndex, endIndex);
			//copyArray(sourceArray, startIndex, endIndex, destArray);
			return;
		}
		
		int half = (startIndex + endIndex) / 2;
		
		MergeSort leftTask = new MergeSort(sourceArray, startIndex, half, destArray);
		MergeSort rightTask = new MergeSort(sourceArray, half + 1, endIndex, destArray);
		
		leftTask.fork();
		rightTask.fork();
		
		rightTask.join();
		leftTask.join();
		
		System.out.printf("Merging pass %d...\n", selfpass);
		
		merge(half);
		
	}
	
	void copyArray (int[] array, int startIndex, int endIndex, int[] destArray) {
		for (int i = startIndex; i <= endIndex; i++) {
			destArray[i] = sourceArray[i];
		}
	}
	
	static void printElements (int[] array, int startIndex, int endIndex) {
		for (int i = startIndex; i <= endIndex; i++) {
			System.out.print(array[i] + " ");
		}
		System.out.print("\n");
	}
	
	static void printArray (int[] array) {
		for (int i = 0; i < array.length; i++) {
			System.out.print(array[i] + " ");
		}
		System.out.print("\n");
	}
	
	static void checkIfSorted (int[] arr) {
		for(int i = 1; i < arr.length; i++) {
			if(arr[i-1] > arr[i]) {
				System.out.print("Array not sorted...\n");
			}
		}
		System.out.print("Array sorted...\n");
	}
	
	public static void main(String[] args) {
			
		int[] inputArray = {5,43,54,24,36,37,23,12,8,7,9,5,48,84,52,19};
		
		int[] outputArray = new int[inputArray.length];
		System.out.printf("Array of %d numbers\n", inputArray.length);
		MergeSort.printArray(inputArray);
		
		ForkJoinPool pool = new ForkJoinPool();
		
		MergeSort task = new MergeSort(inputArray, 0, inputArray.length - 1, outputArray);
		
		pool.invoke(task);
		
		checkIfSorted(outputArray);
		printArray(inputArray);
		printArray(outputArray);
	}
}

