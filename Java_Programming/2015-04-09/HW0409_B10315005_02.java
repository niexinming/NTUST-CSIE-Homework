import java.util.*;

public class HW0409_B10315005_02 {
    public static void selection_sort(double[] arr)
    {
        for (int tail = arr.length - 1; tail > 0; tail--) {
            int p = tail;
            for (int i = 0; i <= tail; i++)
                if (arr[i] > arr[p])
                    p = i;
            if (p != tail) {
                double t = arr[p];
                arr[p] = arr[tail];
                arr[tail] = t;
            }
        }
    }

    public static void print_arr(double[] arr)
    {
        for (int i = 0; i < arr.length; i++)
            System.out.printf("%-8f ", arr[i]);
        System.out.println();
    }

    public static void main(String[] args)
    {
        Scanner s = new Scanner(System.in);
        double[] A = new double[10];
        int i = 0;
        System.out.println("Please input 10 numbers: ");
        while (i < 10) A[i++] = s.nextDouble();
        s.close();
        System.out.println("This is your input");
        print_arr(A);
        selection_sort(A);
        System.out.println("After sorted");
        print_arr(A);
    }
}
