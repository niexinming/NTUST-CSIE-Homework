import java.util.*;

public class HW0409_B10315005_04
{
    public static int partition(int[] L)
    {
        int p = L[0];
        int[] N = new int[L.length];
        int l = 0, r = L.length - 1;
        for (int i = 1; i < L.length; i++) {
            if (L[i] < p) {
                N[l++] = L[i];
            } else {
                N[r--] = L[i];
            }
        }
        N[l] = p;
        for (int i = 0; i < L.length; i++) {
            L[i] = N[i];
        }
        return l;
    }

    public static void print_arr(int[] arr)
    {
        for (int i = 0; i < arr.length; i++)
            System.out.printf("%-8d ", arr[i]);
        System.out.println();
    }

    public static void main(String[] args)
    {
        Scanner s = new Scanner(System.in);
        System.out.println("How many integers?");
        int N = s.nextInt();
        int[] list = new int[N];

        System.out.println("Please input " + list.length + " integers:");
        for (int i = 0; i < list.length; i++)
            list[i] = s.nextInt();
        System.out.println("Contents of list:");
        print_arr(list);
        int p = partition(list);
        System.out.printf("After partition, pivot = %d:\n", p);
        print_arr(list);
    }
}
