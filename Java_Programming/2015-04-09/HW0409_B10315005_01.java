import java.util.*;

public class HW0409_B10315005_01 {
    public static void bubble_sort(double[] arr)
    {
        for (int i = 0; i < arr.length; i++) {
            for (int j = i + 1; j < arr.length; j++) {
                if (arr[j - 1] > arr[j]) {
                    double t = arr[j-1];
                    arr[j-1] = arr[j];
                    arr[j] = t;
                }
            }
        }
    }

    public static void print_arr(double[] arr)
    {
        for (int i = 0; i < arr.length; i++)
            System.out.printf("%f ", arr[i]);
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
        bubble_sort(A);
        System.out.println("After sorted");
        print_arr(A);
    }
}
