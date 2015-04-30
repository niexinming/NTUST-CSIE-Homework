import java.util.*;

public class HW0430_B10315005_04
{
    public static void main(String[] args)
    {
        MyString1 ms = new MyString1("BlabCdefg What tHe stuPid prOblem".toCharArray());
        MyString1 ms2 = new MyString1(ms.toCharArray());
        MyString1 ms3 = ms.substring(5, 9);
        System.out.println("ms = " + ms);
        System.out.println("ms.length() = " + ms.length());
        System.out.println("ms.substring(3, 8) = " + ms.substring(3, 8));
        System.out.println("ms.equals(ms2) = " + ms.equals(ms2));
        System.out.println("ms2.equals(ms3) = " + ms2.equals(ms3));
        System.out.println("ms.tolowercase() = " + ms.toLowerCase());
        System.out.println("MyString1.valueOf(888) = " + MyString1.valueOf(888));
    }
}
