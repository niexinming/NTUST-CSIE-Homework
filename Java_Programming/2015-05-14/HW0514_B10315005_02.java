import java.util.*;

public class HW0514_B10315005_02
{
    public static void sort(ArrayList<Integer> list)
    {
        for(int i = 0; i < list.size(); i++) {
            for(int j = i + 1; j < list.size(); j++) {
                if(list.get(i) < list.get(j)) {
                    int t = list.get(i);
                    list.set(i, list.get(j));
                    list.set(j, t);
                }
            }
        }
    }

    public static void main(String[] args)
    {
        ArrayList<Integer> list = new ArrayList<Integer>();
        Scanner s = new Scanner(System.in);
        for(int i = 0; i < 5; i++) {
            list.add(s.nextInt());
        }
        sort(list);
        for(int i = 0; i < 5; i++) {
            System.out.println(list.get(i));
        }

    }
}
