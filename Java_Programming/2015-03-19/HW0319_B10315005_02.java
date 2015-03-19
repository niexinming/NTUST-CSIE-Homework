import java.util.*;

public class HW0319_B10315005_02
{
    public static void main(String[] args)
    {
        for (int i = 2; i < 10000; i++) {
            int sum = 1;

            for (int j = 2; j < i; j++)
                if (i % j == 0)
                    sum += j;

            if (sum == i) {
                System.out.println(i);
            }
        }
    }
}
