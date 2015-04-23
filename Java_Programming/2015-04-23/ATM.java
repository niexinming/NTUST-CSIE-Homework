import java.util.*;

public class ATM
{
    private Map<Integer , Account> accounts;
    private Scanner input;

    private static final int ACT_CHECK    = 1;
    private static final int ACT_WITHDRAW = 2;
    private static final int ACT_DEPOSIT  = 3;
    private static final int ACT_EXIT     = 4;

    public ATM(Map<Integer, Account> accounts, Scanner scanner)
    {
        this.accounts = accounts;
        this.input = scanner;
    }

    public void run()
    {
        Account acc;
        while(true) {
            acc = this.selectAccount();
            if (acc == null) {
                System.out.println("ID not exists, exit");
                break;
            }
            this.interactive(acc);
        }
    }

    private Account selectAccount()
    {
        System.out.print("Enter a id: ");
        int id = this.input.nextInt();
        return this.accounts.get(id);
    }

    private void interactive(Account acc)
    {
        while(true) {
            int choice = this.menu();
            switch(choice) {
                case ACT_CHECK:
                    System.out.printf("The balance is %f\n", acc.getBalance());
                    break;
                case ACT_WITHDRAW:
                    System.out.print("Enter an amount to withdraw: ");
                    double w = this.input.nextDouble();
                    acc.withdraw(w);
                    break;
                case ACT_DEPOSIT:
                    System.out.print("Enter an amount to deposit: ");
                    double d = this.input.nextDouble();
                    acc.deposit(d);
                    break;
                case ACT_EXIT:
                    return;
                default:
                    System.out.println("Unknow operation");
            }
        }
    }

    private int menu()
    {
        System.out.print("Main menu\n1: check balance\n2: withdraw\n3: deposit\n4: exit\n\nEnter a choice: ");
        return this.input.nextInt();
    }
}
