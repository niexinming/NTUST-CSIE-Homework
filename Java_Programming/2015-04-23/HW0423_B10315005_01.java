import java.util.*;
import java.text.*;

public class HW0423_B10315005_01
{
    public static void stat(Account acc)
    {
        System.out.printf("Account id %d with balance %.2f annual interest rate %.1f%% created at %s\n",
                acc.getId(), acc.getBalance(), acc.getAnnualInterestRate() * 100, (new SimpleDateFormat("yyyy/MM/dd")).format(acc.getDateCreated()));
    }

    public static void main(String[] args)
    {
        Account acc = new Account(1122, 2e4);
        acc.setAnnualInterestRate(0.045);
        stat(acc);
        acc.withdraw(2e4);
        stat(acc);
        acc.deposit(3e3);
        stat(acc);
    }
}
