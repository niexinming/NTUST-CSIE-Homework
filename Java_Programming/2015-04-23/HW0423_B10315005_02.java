import java.util.*;

public class HW0423_B10315005_02
{
    public static Point readFromStdin(String pname, Scanner s)
    {
        System.out.print("Input coordinate for " + pname + ":");
        double x = s.nextDouble();
        double y = s.nextDouble();
        return new Point(x, y);
    }

    public static void main(String[] args)
    {
        Scanner s = new Scanner(System.in);
        Point p1 = readFromStdin("Line 1 Point 1", s);
        Point p2 = readFromStdin("Line 1 Point 2", s);
        Point p3 = readFromStdin("Line 2 Point 1", s);
        Point p4 = readFromStdin("Line 2 Point 2", s);

        Line l1 = new Line(p1, p2);
        Line l2 = new Line(p3, p4);

        // System.out.printf("l1 -> %f, %f\n", l1.m, l1.c);
        // System.out.printf("l2 -> %f, %f\n", l2.m, l2.c);

        if (l1.isIntersectWith(l2))
        {
            Point intp = l1.calculateIntersectPoint(l2);
            System.out.println("Intersect point is at (" + intp.x + ", " + intp.y + ")");
        } else {
            System.out.println("There is no intersect point with two line");
        }
    }
}
