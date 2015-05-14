import java.util.*;

public class HW0514_B10315005_01
{
    public static Integer min(ArrayList<Integer> list)
    {
        if(list == null || list.isEmpty()) {
            return null;
        }
        Integer value = list.get(0);
        for(int i = 1; i < list.size(); i++) {
            if(list.get(i) < value) { value = list.get(i);
            }
        }
        return value;
    }

    public static void main(String[] args)
    {
        ArrayList<Integer> integers = new ArrayList<Integer>();
        Scanner s = new Scanner(System.in);
        while(true) {
            int a = s.nextInt();
            if(a != 0)
                integers.add(a);
            else
                break;
        }
        System.out.println(min(integers));
        System.out.println(min(null));
    }
}
