import java.util.Scanner;

public class HW0305_B10315005_02
{
    public static void main(String[] args)
    {
        Scanner scanner = new Scanner(System.in);

        System.out.print("Enter a number between 0 and 1000: ");
        String row = scanner.nextLine();
        int num = Integer.parseInt(row);
        int l = row.length();
        int n = 1;
        scanner.close();

        if (num < 0 || num > 999) {
            System.out.println("Number must between 0 and 999");
            return;
        }

        for (int i = 0; i < l; i++) {
            char ch = row.charAt(i);
            if ('0' <= ch && ch <= '9') {
                n *= ch - '0';
            }
        }

        System.out.println("The multiplication of all digits in 999 is " + n);
    }
}
