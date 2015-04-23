import java.util.*;

public class HW0423_B10315005_04
{
    public static void main(String[] args)
    {
        Map<Integer, Account> accounts = new HashMap();
        for (int i = 0; i < 10; i++) {
            Account acc = new Account();
            acc.setId(i);
            accounts.put(i, acc);
        }
        (new ATM(accounts, new Scanner(System.in))).run();
    }
}
