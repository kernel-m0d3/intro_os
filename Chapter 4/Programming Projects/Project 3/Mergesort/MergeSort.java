/*

Mergesort Sorting Application

*/

import java.util.Arrays;
import java.util.concurrent.*;

public class MergeSort extends RecursiveAction {
	private int[] sourceArray;
	private int[] destArray;
	private int startIndex;
	private int arrLength;
	
	public MergeSort (int[] src, int start, int length, int[] dest) {
		sourceArray = src;
		startIndex = start;
		arrLength = length;
		destArray = dest;
	}
	
	protected void sortDirectly (int start, int length) {
		Arrays.sort(sourceArray, start, length + start);
	}
	
	protected void merge (int half) {
		int leftTop = 0;
		int rightTop = half;
		
		for (int i = 0; (leftTop < half || rightTop < arrLength) && i < destArray.length; i++) {
			if(sourceArray[leftTop] < sourceArray[rightTop]) {
				destArray[i] = sourceArray[leftTop];
				if(leftTop == half -1) {
					sourceArray[leftTop] = (int)Double.POSITIVE_INFINITY;
				} else {
					leftTop++;
				}
			} else {
				destArray[i] = sourceArray[rightTop];
				if(rightTop == arrLength - 1) {
					sourceArray[rightTop] = (int)Double.POSITIVE_INFINITY;
				} else {
					rightTop++;
				}
			}
		}
	}
	
	protected static int sThreshold = 10;
	
	@Override
	protected void compute() {
		if(arrLength < sThreshold) {
			sortDirectly(startIndex, arrLength);
			return;
		}
		
		int half = (startIndex + arrLength) / 2;
		
		MergeSort leftTask = new MergeSort(sourceArray, startIndex, half, destArray);
		MergeSort rightTask = new MergeSort(sourceArray, startIndex + half, arrLength - half, destArray);
		
		leftTask.fork();
		rightTask.fork();
		
		rightTask.join();
		leftTask.join();
		
		merge(half);
		
	}
	
	static void printArray (int[] array, int length) {
		for (int i = 0; i < length; i++) {
			System.out.print(array[i] + " ");
		}
		System.out.print("\n");
	}

	public static void main(String[] args) {
		
		int[] inputArray = {5,43,54,24,36,37,23,12,8,7,5,3,5,6,9};
		
		int[] outputArray = new int[inputArray.length];
		
		printArray(inputArray, inputArray.length);
		
		ForkJoinPool pool = new ForkJoinPool();
		
		MergeSort task = new MergeSort(inputArray, 0, inputArray.length, outputArray);
		
		pool.invoke(task);
		
		printArray(outputArray, outputArray.length);
		
		
	}
}

