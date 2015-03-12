import java.text.DateFormatSymbols;
import java.util.Locale;

public class HW0312_B10315005_01
{
    public static void main(String[] args)
    {
        String[] months = new DateFormatSymbols(Locale.ENGLISH).getMonths();
        int rnd = (int)(Math.random() * 12);
        System.out.println(months[rnd]);
    }
}
