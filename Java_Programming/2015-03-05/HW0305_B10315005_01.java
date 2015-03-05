import java.util.Scanner;

public class HW0305_B10315005_01 {

    public static void main(String[] args)
    {
        Scanner scanner = new Scanner(System.in);

        System.out.print("Enter the subtotal and a gratuity rate: ");
        double subtotal = scanner.nextDouble();
        double gratuity_rate = scanner.nextDouble() / 100;
        double gratuity = subtotal * gratuity_rate;
        double total = subtotal + gratuity;
        scanner.close();

        System.out.printf("The gratuity is $%g and total is $%g\n", gratuity, total);
    }
}
