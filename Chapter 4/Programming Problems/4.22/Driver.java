class Average implements Runnable {
	public void run () {
		int ave = 0;
		try {
			for(int i = 0; i < Driver.INPUT_LEN; i++) {
				ave += Driver.INPUT[i];
			}
			Driver.AVE = (float)ave / Driver.INPUT_LEN;
		} catch (Exception e) {
			System.out.println ("Exception is caught" + e);
		}
	}
}

class Maximum implements Runnable {
	public void run () {
		int max = Driver.MIN_INT_VALUE;
		try {
			for(int i = 0; i < Driver.INPUT_LEN; i++) {
				max = max > Driver.INPUT[i] ? max : Driver.INPUT[i];
			}
			Driver.MAX = max;
		} catch (Exception e) {
			System.out.println ("Exception is caught" + e);
		}
	}
}

class Minimum implements Runnable {
	public void run () {
		int min = Driver.MAX_INT_VALUE;
		try {
			for(int i = 0; i < Driver.INPUT_LEN; i++) {
				min = min < Driver.INPUT[i] ? min : Driver.INPUT[i];
			}
			Driver.MIN = min;
		} catch (Exception e) {
			System.out.println ("Exception is caught" + e);
		}
	}
}

public class Driver {

	public static final int ARR_SIZE = 255;
	public static final int NUM_THREADS = 3;
	public static final int MAX_INT_VALUE = 2147483647;
	public static final int MIN_INT_VALUE = -2147483648;
	public static int INPUT[] = new int[ARR_SIZE];
	public static int INPUT_LEN;
	public static int MAX;
	public static int MIN;
	public static float AVE;

	static void parseInput (String[] args) {
		INPUT_LEN = args.length;
		for (int i = 0; i < INPUT_LEN; i++) {
			INPUT[i] = Integer.parseInt(args[i]);
		}
	}

	static void printInput () {
		for (int i = 0; i < INPUT_LEN; i++) {
			System.out.print(INPUT[i] + " ");
		}
		System.out.print("\n");
	}

	public static void main (String[] args) {
		if (args.length == 0) {
			System.out.println("Input numbers after the command...");
			System.out.println("example: \'java Driver 1 2 3 6 5\'");
			System.exit(1);
		}
		parseInput(args);

		Thread average = new Thread(new Average());
        average.start();
        Thread maximum = new Thread(new Maximum());
        maximum.start();
        Thread minimum = new Thread(new Minimum());
        minimum.start();

        try {
            average.join();
            maximum.join();
            minimum.join();
        } catch(Exception e) {
            System.out.println("Exception is caught" + e);
        }

        System.out.printf("The average value is: %.2f\n", AVE);
        System.out.println("The minimum value is: " + MIN);
        System.out.println("The maximum value is: " + MAX);
	}
}
