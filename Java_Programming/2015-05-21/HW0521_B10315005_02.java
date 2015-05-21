import java.util.*;

public class HW0521_B10315005_02
{
    public static void sort(ArrayList<Number> list)
    {
        for (int i = 0; i < list.size(); i++) {
            for (int j = i + 1; j < list.size(); j++) {
                Number I = list.get(i);
                Number J = list.get(j);
                double a = I.doubleValue();
                double b = J.doubleValue();
                if (a > b) {
                    list.set(i, J);
                    list.set(j, I);
                }
            }
        }

        for (int k = 0; k < list.size(); k++) {
            System.out.print(list.get(k).intValue() + " ");
        }
    }
    public static void main(String[] args)
    {
        ArrayList<Number> list = new ArrayList<Number>();
        list.add(new Integer(123));
        list.add(new Integer(2015));
        list.add(new Integer(127));
        list.add(new Integer(33));
        list.add(new Integer(21));
        list.add(new Integer(5));
        list.add(new Integer(389));
        list.add(new Integer(5916));
        sort(list);
    }
}
