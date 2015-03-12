import java.util.*;

public class HW0312_B10315005_04
{
    public static Rectangle readRect(Scanner s)
    {
        double x = s.nextDouble();
        double y = s.nextDouble();
        double w = s.nextDouble();
        double h = s.nextDouble();
        Rectangle r = new Rectangle(x, y, w, h);
        return r;
    }

    public static void main(String[] args)
    {
        Scanner s = new Scanner(System.in);
        System.out.print("Enter r1's center x-, y-coordinates, width, and height: ");
        Rectangle r1 = readRect(s);
        System.out.print("Enter r2's center x-, y-coordinates, width, and height: ");
        Rectangle r2 = readRect(s);

        if (r1.equals(r2)) {
            System.out.println("r1 and r2 are overlapped (r1, r2 are same)");
        } else if (r1.in_the(r2)) {
            System.out.println("r1 in the r2");
        } else if (r2.in_the(r1)) {
            System.out.println("r2 in the r1");
        } else if (r1.out_of(r2)) {
            System.out.println("r1 and r2 are not overlapped");
        } else {
            System.out.println("r1 and r2 are overlapped");
        }
    }
}
