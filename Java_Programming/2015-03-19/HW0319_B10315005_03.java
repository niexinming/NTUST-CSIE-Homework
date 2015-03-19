import java.util.*;

public class HW0319_B10315005_03
{
    public static void main(String[] args)
    {
        double sum = 0;
        double a = 1, b;
        for (int i = 1; i < 1000; i++) {
            b = Math.sqrt(i+1);
            sum += 1 / (a+b);
            a = b;
        }
        System.out.println(sum);
    }
}
