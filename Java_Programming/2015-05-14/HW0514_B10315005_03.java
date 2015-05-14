import java.util.*;

public class HW0514_B10315005_03
{
    public static void sameElements(ArrayList<Integer> list)
    {
        HashMap<Integer, Integer> map = new HashMap<Integer, Integer>();
        for(int i = 0; i < list.size(); i++) {
            if(!map.containsKey(list.get(i))) {
                map.put(list.get(i), 0);
            }
            map.put(list.get(i), map.get(list.get(i)) + 1);
        }
        System.out.print("Duplicated items: ");
        for(int i = 0; i < list.size(); i++) {
            if(map.get(list.get(i)) > 1) {
                System.out.print(list.get(i) + " ");
            }
        }

    }
    public static void main(String[] args)
    {
        ArrayList<Integer> list = new ArrayList<Integer>();
        Scanner s = new Scanner(System.in);
        System.out.print("Input 20 integers: ");
        for(int i = 0; i < 20 ; i++) {
            list.add(s.nextInt());
        }
        sameElements(list);
    }
}
