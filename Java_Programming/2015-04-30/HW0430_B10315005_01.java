import java.util.*;

public class HW0430_B10315005_01
{
    public static void main(String[] args)
    {
        Scanner s = new Scanner(System.in);
        System.out.print("Color: ");
        String color = s.next();
        System.out.print("Filled (y/n): ");
        boolean filled = s.next().toLowerCase().charAt(0) == 'y';
        System.out.print("Side1: ");
        double s1 = s.nextDouble();
        System.out.print("Side2: ");
        double s2 = s.nextDouble();
        System.out.print("Side3: ");
        double s3 = s.nextDouble();

        Triangle tri = new Triangle(s1, s2, s3);
        tri.setColor(color);
        tri.setFilled(filled);
        System.out.println(tri.toString());
    }
}
