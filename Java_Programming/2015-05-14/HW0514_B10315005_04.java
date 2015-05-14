import java.util.*;

public class HW0514_B10315005_04
{
    public static void main(String[] args)
    {
        ArrayList<Integer> list = new ArrayList<Integer>();
        Scanner s = new Scanner(System.in);
        int p = (int)(Math.random() * 10);
        int q = (int)(Math.random() * 10);
        int v = p * q;
        System.out.printf("What is %d x %d?\n", p, q);
        while(true) {
            int r = s.nextInt();
            if(r == v) {
                System.out.println("That's right!");
                break;
            } else {
                if(list.contains(r)) {
                    System.out.println("Idiot! You have already tried that.");
                } else {
                    list.add(r);
                    System.out.println("No, try again.");
                }
            }
        }
    }
}
