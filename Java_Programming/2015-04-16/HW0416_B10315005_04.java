public class HW0416_B10315005_04
{
    public static void main(String[] args)
    {
        System.out.println("Construct a Rectangle with empty constructor...");
        Rectangle rect = new Rectangle();
        rect.selfTest();
        System.out.println("Construct a Rectangle with width 12.5, height 16...");
        Rectangle rect2 = new Rectangle(12.5, 16);
        rect2.selfTest();
    }
}
