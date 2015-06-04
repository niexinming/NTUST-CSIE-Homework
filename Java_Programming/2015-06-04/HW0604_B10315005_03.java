import java.util.*;

public class HW0604_B10315005_03
{
    public static <E extends Comparable<E>> void sort(ArrayList<E> array)
    {
        for(int i = 0; i < array.size(); i++) {
            for(int j  = i + 1; j < array.size(); j++) {
                E I = array.get(i);
                E J = array.get(j);
                if (I.compareTo(J) > 0) {
                    array.set(i, J);
                    array.set(j, I);
                }
            }
        }
    }

    public static <E> void print(ArrayList<E> list)
    {
        for(int i = 0; i < list.size(); i++) {
            System.out.print(list.get(i) + " ");
        }
    }

    public static void main(String[] args)
    {
        ArrayList<Integer> list = new ArrayList<Integer>();
        list.add(5);
        list.add(8);
        list.add(-6);
        list.add(9);
        list.add(0);
        list.add(3);
        list.add(4);
        list.add(1);
        System.out.println("Before sort:");
        print(list);
        sort(list);
        System.out.println("\nAfter sort:");
        print(list);
    }
}
