import java.util.*;

public class HW0430_B10315005_03
{
    public static String[] split(String s, String regex)
    {
        String[] v = s.split(regex);
        String[] tmp;
        int i;
        List<String> r = new ArrayList();
        for (i = 0; i < v.length; i++)
            if (v[i].length() > 0)
                r.add(v[i]);
        tmp = new String[r.size()];
        r.toArray(tmp);
        v = tmp;

        int[] iv = new int[v.length];
        r = new ArrayList();

        int p = 0;
        for (i = 0; i < v.length; i++) {
            if (v[i].length() == 0)
                continue;
            iv[i] = s.indexOf(v[i], p);
            p = iv[i] + v[i].length();
        }
        p = 0;
        char[] ca = new char[1];
        for (i = 0; i < iv.length; i++) {
            while (p < iv[i]) {
                ca[0] = s.charAt(p++);
                r.add(new String(ca));
            }
            r.add(v[i]);
            p += v[i].length();
        }
        while (p < s.length()) {
            ca[0] = s.charAt(p++);
            r.add(new String(ca));
        }

        tmp = new String[r.size()];
        r.toArray(tmp);
        return tmp;
    }

    public static void main(String[] args)
    {
        String s = "#?ab#ab?ab#c?d#?efg?h#i?#?#j?#";
        String[] r = split(s, "[?#]");
        System.out.println(s);
        System.out.println(String.join(", ", r));
    }
}
