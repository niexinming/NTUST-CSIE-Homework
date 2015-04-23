import java.util.*;

public class Account
{
    private static double annualInterestRate = 0;

    private int id = 0;
    private double balance = 0;
    private Date dateCreated;

    public Account()
    {
        this.dateCreated = new Date();
    }

    public Account(int id, double balance)
    {
        this.id = id;
        this.balance = balance;
        this.dateCreated = new Date();
    }

    public int getId() { return this.id; }
    public void setId(int id) { this.id = id; }

    public double getBalance() { return this.balance; }
    public void setBalance(double balance) { this.balance = balance; }

    public double getAnnualInterestRate() { return Account.annualInterestRate; }
    public void setAnnualInterestRate(double annualInterestRate)
    {
        Account.annualInterestRate = annualInterestRate;
    }

    public Date getDateCreated() { return this.dateCreated; }

    public double getMonthlyInterestRate()
    {
        return this.getAnnualInterestRate() / 12;
    }
    public double getMonthlyInterest()
    {
        return this.getMonthlyInterestRate() * this.getBalance();
    }
    public void deposit(double balance) { this.balance += balance; }
    public void withdraw(double balance) { this.balance -= balance; }
}
