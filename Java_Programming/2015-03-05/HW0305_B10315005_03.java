import java.util.Scanner;

public class HW0305_B10315005_03
{
    public static void main(String[] args)
    {
        Scanner scanner = new Scanner(System.in);

        System.out.print("Enter the number of minutes: ");
        long num  = scanner.nextLong();
        long days = num / (60 * 24);
        long years = days / 365;
        days %= 365;
        System.out.printf("%d minutes is approximately %d years and %d days", num, years, days);
    }
}
