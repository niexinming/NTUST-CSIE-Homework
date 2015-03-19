import java.util.Scanner;

public class HW0319_B10315005_04
{
    public static String toBinary(long x)
    {
        String r = "";
        boolean skip = true;
        for (int i = 0; i < 32; i++) {
            long m = x >> (31 - i);
            m = m & 1;
            if (m == 1) skip = false;
            if (skip) continue;
            r += m == 1 ? "1" : "0";
        }
        return r;
    }

    public static void main(String[] args)
    {
        System.out.print("Input a number convert to binary: ");
        System.out.println(toBinary(new Scanner(System.in).nextInt()));
    }
}
