import java.lang.Thread;
import java.util.*;

public class Application {

    public static void main( String args []) {
        Account a;
        Date d;
        double ret;

        a = new CheckingAccount("John Smith", 1500.0);

        try {
            ret = a.withdraw(100.00);
            System.out.println ("Account <" + a.name() + "> now has $" + ret + " balance\n");
        } catch (Exception e) {
            stdExceptionPrinting(e, a.balance());
        }

        a = new CheckingAccount("John Smith", 1500.0);

        try {
            ret = a.withdraw(600.00);
            System.out.println ("Account <" + a.name() + "> now has $" + ret + " balance\n");
        } catch (Exception e) {
            stdExceptionPrinting(e, a.balance());
        }

        /* put your own tests here ....... */

        Account[] accountList = new Account[] {
            new CheckingAccount("A-Checking-Account", 1800.0),
            new SavingAccount("William Hurt", 1200.0),
            new CDAccount("Woody Allison", 1000.0, new Date(2013 - 1900, 1 - 1, 1)),
            new LoanAccount("Judi Foster", -1500.0)
        };

        // compute interest for all accounts
        for (int count = 0; count < accountList.length; count++) {
            System.out.println("--------------------------");
            System.out.printf("Testing round #%d\n", count);
            System.out.println("--------------------------");
            Account account = accountList[count];
            System.out.printf("- This is a %s class\n", account.getClass().getName());
            double newBalance;

            try {
                System.out.println("- Now call computeInterest()");
                newBalance = account.computeInterest();
                System.out.printf ("- Account <%s> now has $%g balance\n", account.name(), newBalance);
            } catch (Exception e) {
                stdExceptionPrinting(e, account.balance());
            }

            try {
                System.out.println("- Now call deposit() 2 times");
                account.deposit(1500);
                account.deposit(878787);
                System.out.printf ("- Account <%s> now has $%g balance\n", account.name(), account.balance());
            } catch (Exception e) {
                stdExceptionPrinting(e, account.balance());
            }

            try {
                System.out.println("- Now call withdraw() 4 times");
                for(int i = 1; i <= 4; i++) {
                    System.out.printf("- %d time withdraw", i);
                    account.withdraw(i * 80);
                    System.out.printf ("- Account <%s> now has $%g balance\n", account.name(), account.balance());
                }
            } catch (Exception e) {
                stdExceptionPrinting(e, account.balance());
            }

            try {
                System.out.println("- Now call computeInterest()");
                newBalance = account.computeInterest(new Date(2018 - 1900, 8 - 1, 4));
                System.out.printf ("- Account <%s> now has $%g balance\n", account.name(), newBalance);
            } catch (Exception e) {
                stdExceptionPrinting(e, account.balance());
            }

            try {
                System.out.println("- Now call withdraw(999999999)");
                account.withdraw(999999999);
                System.out.printf ("- Account <%s> now has $%g balance\n", account.name(), account.balance());
            } catch (Exception e) {
                stdExceptionPrinting(e, account.balance());
            }

            System.out.println();
        }
    }

    static void stdExceptionPrinting(Exception e, double balance) {
        System.out.printf("EXCEPTION: Banking system throws a %s with message: \n\tMESSAGE: %s\n",
                e.getClass(), e.getMessage());
        System.out.println("Stack Trace:");
        e.printStackTrace();
        System.out.println("\tAccount balance remains $" + balance + "\n");
    }
}

// vim: et cc=100
