import java.util.*;

public class HW0319_B10315005_01
{
    public static void main(String[] main)
    {
        System.out.print(" ____ ___    ____      _            _       _             \n|  _ \\_ _|  / ___|__ _| | ___ _   _| | __ _| |_ ___  _ __ \n| |_) | |  | |   / _` | |/ __| | | | |/ _` | __/ _ \\| '__|\n|  __/| |  | |__| (_| | | (__| |_| | | (_| | || (_) | |   \n|_|  |___|  \\____\\__,_|_|\\___|\\__,_|_|\\__,_|\\__\\___/|_|   \n                                                          \n\n");
        System.out.print("Input how many iterations? ");
        Scanner s = new Scanner(System.in);
        int iterations = s.nextInt();
        double sum = 0;
        for (int i = 0; i < iterations; i++)
            sum += ((i & 1) == 0 ? 4 : -4) / (double)(i * 2 + 1);
        System.out.printf("Claculate PI for %d iterations is %.12f\n", iterations, sum);
    }
}
