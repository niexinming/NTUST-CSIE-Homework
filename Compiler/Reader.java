import java.util.Scanner;
class Reader {
	private static Scanner scanner = null;

	public static Scanner getScanner() {
		if(scanner == null) {
			scanner = new Scanner(System.in);
		}

		return scanner;
	}

	public static int readInteger() {
		return getScanner().nextInt();
	}

	public static double readDouble() {
		return getScanner().nextDouble();
	}

	public static String readLine() {
		return getScanner().nextLine();
	}
}
