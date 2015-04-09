import java.util.*;

public class HW0409_B10315005_03
{
    public static boolean equals(int[] l1, int[] l2) {
        if (l1.length != l2.length)
            return false;
        int n1[] = Arrays.copyOf(l1, l1.length);
        int n2[] = Arrays.copyOf(l2, l2.length);
        Arrays.sort(n1);
        Arrays.sort(n2);
        return Arrays.equals(n1, n2);
    }

    public static void print_arr(int[] arr)
    {
        for (int i = 0; i < arr.length; i++)
            System.out.printf("%-8d ", arr[i]);
        System.out.println();
    }

    public static void main(String[] args)
    {
        int[] l1 = new int[5],
              l2 = new int[5];

        Scanner s = new Scanner(System.in);
        System.out.println("Please input 5 integers for list1:");
        for (int i = 0; i < l1.length; i++)
            l1[i] = s.nextInt();
        System.out.println("Please input 5 integers for list2:");
        for (int i = 0; i < l1.length; i++)
            l2[i] = s.nextInt();
        System.out.println("Contents of list1:");
        print_arr(l1);
        System.out.println("Contents of list2:");
        print_arr(l2);
        System.out.println("list1 and list2 are " + (equals(l1, l2) ? "" : "not ") + "identical");
    }
}
