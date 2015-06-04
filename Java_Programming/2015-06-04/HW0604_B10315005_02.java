import java.util.*;

public class HW0604_B10315005_02
{
    public static <E extends Comparable<E>> void shuffle(ArrayList<E> array)
    {
        for(int i = 0; i < array.size(); i++) {
            int j = (int)(Math.random() * array.size());
            E I = array.get(i);
            E J = array.get(j);
            array.set(i, J);
            array.set(j, I);
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
        for(int i = 0; i < 10; i++) {
            list.add(i);
        }
        System.out.println("Before shuffle:");
        print(list);
        shuffle(list);
        System.out.println("\nAfter shuffle:");
        print(list);
    }
}
