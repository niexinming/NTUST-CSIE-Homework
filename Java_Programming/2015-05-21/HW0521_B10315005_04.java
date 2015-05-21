import java.util.*;

public class HW0521_B10315005_04
{
    public static void main(String[] args)
    {
        Rational n = new Rational(0);
        for (int i = 2; i <= 100; i += 2) {
            Rational m = new Rational(i - 1, i);
            n = n.add(m).simplify();
        }
        System.out.println(n);
    }
}
