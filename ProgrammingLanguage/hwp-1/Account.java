import java.util.*;

class BankingException extends Exception {
    BankingException () { super(); }
    BankingException (String s) { super(s); }
}

interface BasicAccount {
    String name();
    double balance();
}

interface WithdrawableAccount extends BasicAccount {
    double withdraw(double amount) throws BankingException;
}

interface DepositableAccount extends BasicAccount {
    double deposit(double amount) throws BankingException;
}

interface InterestableAccount extends BasicAccount {
    double computeInterest() throws BankingException;
}

interface FullFunctionalAccount extends WithdrawableAccount,
                                        DepositableAccount,
                                        InterestableAccount {
}

public abstract class Account {

    // protected variables to store commom attributes for every bank accounts
    protected String accountName;
    protected double accountBalance;
    protected double accountInterestRate;
    protected Date openDate;
    protected Date lastInterestDate;

    // public methods for every bank accounts
    public String name() {
        return accountName;
    }

    public double balance() {
        return accountBalance;
    }

    public double deposit(double amount) throws BankingException {
        accountBalance += amount;
        return accountBalance;
    }

    abstract double withdraw(double amount, Date withdrawDate) throws BankingException;

    public double withdraw(double amount) throws BankingException {
        Date withdrawDate = new Date();
        return withdraw(amount, withdrawDate);
    }

    abstract double computeInterest(Date interestDate) throws BankingException;

    public double computeInterest() throws BankingException {
        Date interestDate = new Date();
        return computeInterest(interestDate);
    }
}

/*
 *  Derived class: CheckingAccount
 *
 *  Description:
 *      Interest is computed daily; there's no fee for
 *      withdraw; there is a minimum balance of $1000.
 */

class CheckingAccount extends Account implements FullFunctionalAccount {

    CheckingAccount(String s, double firstDeposit) {
        accountName = s;
        accountBalance = firstDeposit;
        accountInterestRate = 0.12;
        openDate = new Date();
        lastInterestDate = openDate;
    }

    CheckingAccount(String s, double firstDeposit, Date firstDate) {
        accountName = s;
        accountBalance = firstDeposit;
        accountInterestRate = 0.12;
        openDate = firstDate;
        lastInterestDate = openDate;
    }

    public double withdraw(double amount, Date withdrawDate) throws BankingException {
        // minimum balance is 1000, raise exception if violated
        if ((accountBalance  - amount) < 1000) {
            throw new BankingException ("Underfraft from checking account name:" +
                    accountName);
        } else {
            accountBalance -= amount;
            return accountBalance;
        }
    }

    public double computeInterest (Date interestDate) throws BankingException {
        if (interestDate.before(lastInterestDate)) {
            throw new BankingException ("Invalid date to compute interest for account name: " +
                                        accountName);
        }

        int numberOfDays = (int) ((interestDate.getTime() - lastInterestDate.getTime())
                / 86400000.0);
        System.out.println("* Number of days since last interest is " + numberOfDays);
        double interestEarned = (double) numberOfDays / 365.0 *
            accountInterestRate * accountBalance;
        System.out.println("+ Interest earned is " + interestEarned);
        lastInterestDate = interestDate;
        accountBalance += interestEarned;
        return accountBalance;
    }
}

class SavingAccount extends Account implements FullFunctionalAccount {
    static final int OperationFee = 1;
    int lastWithdrawYearmonth, withdrawCounter;

    SavingAccount(String s, double firstDeposit) {
        accountName = s;
        accountBalance = firstDeposit;
        accountInterestRate = 0.00987987987;
        openDate = new Date();
        lastInterestDate = openDate;
        lastWithdrawYearmonth = 0;
        withdrawCounter = 0;
    }

    SavingAccount(String s, double firstDeposit, Date firstDate) {
        accountName = s;
        accountBalance = firstDeposit;
        accountInterestRate = 0.00987987987;
        openDate = firstDate;
        lastInterestDate = openDate;
        lastWithdrawYearmonth = 0;
        withdrawCounter = 0;
    }

    private boolean checkWithdrawOffer(Date withdrawDate) {
        int yearmonth = withdrawDate.getYear() * 100 + withdrawDate.getMonth();
        return yearmonth > lastWithdrawYearmonth || withdrawCounter < 3;
    }

    private void setLastWithdrawDate(Date withdrawDate) throws BankingException {
        int yearmonth = withdrawDate.getYear() * 100 + withdrawDate.getMonth();
        if (yearmonth > lastWithdrawYearmonth) {
            withdrawCounter = 0;
            lastWithdrawYearmonth = yearmonth;
        } else if (yearmonth < lastWithdrawYearmonth) {
            throw new BankingException("Impossible withdrawDate, earlier than last withdrawDate, " +
                    "account name: " + accountName);
        }
    }

    public double deposit(double amount) throws BankingException {
        double finalAccountBalance = accountBalance + amount - OperationFee;
        if (finalAccountBalance < 0) {
            throw new BankingException ("Underfraft from checking account name:" + accountName);
        }
        accountBalance = finalAccountBalance;
        return accountBalance;
    }

    public double withdraw(double amount, Date withdrawDate) throws BankingException {
        // minimum balance is 0, raise exception if violated
        if (
            ((accountBalance - amount) < 0 && checkWithdrawOffer(withdrawDate)) ||
            ((accountBalance - amount - OperationFee) < 0 && !checkWithdrawOffer(withdrawDate))
        ) {
            throw new BankingException ("Underfraft from checking account name:" + accountName);
        } else {
            if(!checkWithdrawOffer(withdrawDate)) {
                accountBalance -= OperationFee;
            }
            withdrawCounter++;
            accountBalance -= amount;
            System.out.printf("+ Withdraw %g from account %s, now balance is %g\n",
                              amount, accountName, accountBalance);
            return accountBalance;
        }
    }

    public double computeInterest (Date interestDate) throws BankingException {
        System.out.println("* Now computing interest...");
        if (interestDate.before(lastInterestDate)) {
            throw new BankingException ("Invalid date to compute interest for account name: " +
                                        accountName);
        }

        int numberOfDays = (int) ((interestDate.getTime() - lastInterestDate.getTime()) / 86400000.0);
        System.out.println("* Number of days since last interest is " + numberOfDays);

        double interestEarned = (double) numberOfDays / 30.0 * accountInterestRate * accountBalance;
        System.out.println("+ Interest earned is " + interestEarned);
        lastInterestDate = interestDate;
        accountBalance += interestEarned;
        return accountBalance;
    }
}

class CDAccount extends Account implements FullFunctionalAccount {
    static final int OperationFee = 250;
    boolean gotInterest;

    CDAccount(String s, double firstDeposit) {
        accountName = s;
        accountBalance = firstDeposit;
        accountInterestRate = 0.333;
        openDate = new Date();
        lastInterestDate = openDate;
        gotInterest = false;
    }

    CDAccount(String s, double firstDeposit, Date firstDate) {
        accountName = s;
        accountBalance = firstDeposit;
        accountInterestRate = 0.333;
        openDate = firstDate;
        lastInterestDate = openDate;
        gotInterest = false;
    }

    public double deposit(double amount) throws BankingException {
        if (computeDays() < 365) {
            throw new BankingException("Date has not arrived, you can't deposit money to account: " +
                    accountName);
        }
        accountBalance = accountBalance + amount;
        return accountBalance;
    }

    public double withdraw(double amount, Date withdrawDate) throws BankingException {
        // minimum balance is 0, raise exception if violated
        if ((accountBalance - amount - OperationFee) < 0) {
            throw new BankingException ("Underfraft from checking account name:" + accountName);
        } else {
            accountBalance -= amount + OperationFee;
            System.out.printf("+ Withdraw %g from account %s, now balance is %g\n",
                              amount, accountName, accountBalance);
            return accountBalance;
        }
    }

    private int computeDays(Date time) {
        // compute how many days since first deposit
        return (int) ((time.getTime() - lastInterestDate.getTime()) / 86400000.0);
    }

    private int computeDays() {
        return computeDays(new Date());
    }

    public double computeInterest (Date interestDate) throws BankingException {
        System.out.println("* Now computing interest...");
        if (interestDate.before(lastInterestDate)) {
            throw new BankingException ("Invalid date to compute interest for account name:" +
                                        accountName);
        }

        int numberOfDays = computeDays(interestDate);
        System.out.println("* Number of days since last interest is " + numberOfDays);

        if (gotInterest) {
            throw new BankingException("This CDAccount has already gotten it's interest," +
                                       " account name: " + accountName);
        }

        if (numberOfDays < 365) {
            throw new BankingException("A CDAccount won't got interest within 365 days, account: " +
                                       accountName);
        }

        // we only gives you 1-year interest, once
        gotInterest = true;
        double interestEarned = accountInterestRate * accountBalance;
        System.out.println("+ Interest earned is " + interestEarned);
        lastInterestDate = interestDate;
        accountBalance += interestEarned;
        return accountBalance;
    }
}

class LoanAccount extends Account implements DepositableAccount {

    LoanAccount(String s, double firstDeposit) {
        accountName = s;
        accountBalance = firstDeposit;
        openDate = new Date();
        lastInterestDate = openDate;
    }

    LoanAccount(String s, double firstDeposit, Date firstDate) {
        accountName = s;
        accountBalance = firstDeposit;
        openDate = firstDate;
        lastInterestDate = openDate;
    }

    public double deposit(double amount) throws BankingException {
        double balance = accountBalance + amount;
        if (balance > 0) {
            throw new BankingException("You paid too much for loan account: " + accountName);
        } else if (balance == 0) {
            System.out.println("+ Congratulations, you have paid off your debts!");
        }
        accountBalance = balance;
        return accountBalance;
    }

    public double withdraw(double amount, Date withdrawDate) throws BankingException {
        throw new BankingException("You can not withdraw from a loan account");
    }

    public double computeInterest(Date interestDate) throws BankingException {
        throw new BankingException("Loan account do not have interest");
    }
}

// vim: et cc=100
