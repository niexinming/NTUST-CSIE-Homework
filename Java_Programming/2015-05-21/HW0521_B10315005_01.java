import java.util.*;

public class HW0521_B10315005_01
{
    public static void average(ArrayList<Integer> list)
    {
        double n = 0;
        for(int i = 0; i < list.size(); i++) {
            n += list.get(i);
        }
        System.out.println("Average is " + n / list.size());
    }
    public static void main(String[] args)
    {
        ArrayList<Integer> list = new ArrayList<Integer>();
        list.add(123);
        list.add(2015);
        list.add(127);
        list.add(33);
        list.add(21);
        list.add(5);
        list.add(389);
        list.add(5916);
        average(list);
    }
}
