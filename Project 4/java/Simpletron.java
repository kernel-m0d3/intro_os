import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;

class Simpletron {

	private static int ACC = 0;
	private static int[] MEMORY = new int[100];
	private static int programCounter = 0;
	private static boolean readInstructions = true;

	private static void interactive () {
		System.out.println("Enter instruction codes. when finished, input a negative number");
		int inputCode = 0;
		BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
		while (readInstructions) {
			try {
				System.out.print("::#");
				String input = reader.readLine();
				inputCode = Integer.parseInt(input);
				
				if (inputCode < 0) {
					return;
				} else if (inputCode < 1000 || inputCode > 4300) {
					System.out.println("try again...");
				} else {
					MEMORY[programCounter] = inputCode;
					programCounter++;
				}
			} catch (Exception e) {
				System.out.println("input error: " + e);
			}
		}
	}
	
	static void printInput () {
		for (int i = 0; i < MEMORY.length && MEMORY[i] != 0; i++) {
			System.out.print(MEMORY[i] + " ");
		}
		System.out.print("\n");
	}
	
	static String askUserInput (String text) {
		System.out.print(text);
		BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
		String input = "weird";
		try {
			input = reader.readLine();
			return input;
		} catch (Exception e) {
			System.out.println("input error: " + e);
		}
		return input;
	}
	
	static int convertStringToInt (String input) {
		int returnInt = 0;
		try {
			returnInt = Integer.parseInt(input);
		} catch (Exception e) {
			System.out.println("input error: " + e);
		}
		return returnInt;
	}
	
	static void executeFromMemory () {
		programCounter = 0;
		while (MEMORY[programCounter] !=0) {
			executeLine(MEMORY[programCounter]);
			programCounter++;
		}
	}
	
	static void executeLine (int code) {
		int opCode = code / 100;
		int operand = code % 100;
		switch (opCode) {
			case 40:
				programCounter = operand;
				executeLine(MEMORY[programCounter]);
				break;
			case 41:
				if (ACC < 0) {
					programCounter = operand;
					executeLine(MEMORY[programCounter]);
				}
				break;
			case 42:
				if (ACC == 0) {
					programCounter = operand;
					executeLine(MEMORY[programCounter]);
				}
				break;
			case 43:
				System.exit(0);
				break;
			case 30:
				ACC += MEMORY[operand];
				break;
			case 31:
				ACC -= MEMORY[operand];
				break;
			case 32:
				ACC *= MEMORY[operand];
				break;
			case 33:
				ACC /= MEMORY[operand];
				break;
			case 10:
				String read = askUserInput("Read: ");
				int readCode = convertStringToInt(read);
				MEMORY[operand] = readCode;
				break;
			case 11:
				System.out.println(MEMORY[operand]);
				break;
			case 20:
				ACC = MEMORY[operand];
				break;
			case 21:
				MEMORY[operand] = ACC;
				break;
		}
	}
	
	private static void openSMLFile(String filename) {
		try {
			File f=new File(filename);     
			FileReader fr=new FileReader(f);   
			BufferedReader br=new BufferedReader(fr);  
			String s=null;  
			int i = 0;        
			while ((s=br.readLine())!=null) {
				System.out.println(s);
				MEMORY[i] = convertStringToInt(s);
				i++;
			}
			fr.close();
		} catch(Exception e) {
			System.out.println("file open error: " + e);
		}
	}
	
	public static void main (String[] args) {
		if (args.length == 0) {
			System.out.println("No file entered. Entering interactive mode...");
			interactive();
			//printInput();
			System.out.println("Executing...");
			executeFromMemory();
			
		} else {
			openSMLFile(args[0]);
			printInput();
			executeFromMemory();
		}
		
	}
}
