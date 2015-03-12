import java.util.*;

public class HW0312_B10315005_03
{
    public static double distance(double x0, double y0, double x1, double y1)
    {
        return Math.sqrt(Math.pow(x0 - x1, 2) + Math.pow(y0 - y1, 2));
    }

    public static void main(String[] args)
    {
        final double x0 = 0;
        final double y0 = 0;
        final double radius = 10;

        Scanner s = new Scanner(System.in);

        System.out.print("Enter a point with two coordinates: ");
        double x = s.nextDouble();
        double y = s.nextDouble();

        String state = "";
        double d = distance(x0, y0, x, y);
        if (d > radius) state = "out of";
        else if (d == radius) state = "on";
        else state = "in";
        System.out.printf("Point (%f, %f) is %s the circle.\n", x, y, state);
    }
}
