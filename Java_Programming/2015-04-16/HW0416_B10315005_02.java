import java.util.*;

public class HW0416_B10315005_02
{
    public static void main(String[] args)
    {
        System.out.println("Input a number between 0 and 511: ");
        int n = (new Scanner(System.in).nextInt());
        if (n < 0 || n > 511) {
            System.out.println("Not a valid number.");
            return;
        }
        for (int i = 0; i < 9; i++) {
            char c = (((n >> (8 - i)) & 1) == 1) ? 'T' : 'H';
            System.out.print(c);
            if (i % 3 == 2)
                System.out.println();
        }
    }
}
