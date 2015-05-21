import java.math.BigInteger;

public class Rational
{
    public BigInteger a;
    public BigInteger b;

    public Rational(long a)
    {
        this.a = BigInteger.valueOf(a);
        this.b = BigInteger.ONE;
    }

    public Rational(BigInteger a)
    {
        this.a = a;
        this.b = BigInteger.ONE;
    }

    public Rational(long a, long b)
    {
        this.a = BigInteger.valueOf(a);
        this.b = BigInteger.valueOf(b);
    }

    public Rational(BigInteger a, BigInteger b)
    {
        this.a = a;
        this.b = b;
    }

    public Rational negate()
    {
        return new Rational(this.a.negate(), this.b);
    }

    public Rational add(Rational other)
    {
        return new Rational(
                this.a.multiply(other.b).add(other.a.multiply(this.b)),
                this.b.multiply(other.b)
                ).simplify();
    }

    public Rational sub(Rational other)
    {
        return this.add(other.negate());
    }

    public Rational inverse()
    {
        return new Rational(this.b, this.a);
    }

    public Rational multiply(Rational other)
    {
        return new Rational(this.a.multiply(other.a),
                this.b.multiply(other.b)).simplify();
    }

    public Rational divide(Rational other)
    {
        return this.multiply(other.inverse());
    }

    public Rational simplify()
    {
        BigInteger n = this.a.gcd(this.b);
        if (this.a.compareTo(BigInteger.ZERO) == 0 ||
                this.b.compareTo(BigInteger.ZERO) == 0)
            return this;
        this.a = this.a.divide(n);
        this.b = this.b.divide(n);
        return this;
    }

    public Rational normalize()
    {
        if(this.b.signum() == -1) {
            this.a = this.a.negate();
            this.b = this.b.negate();
        }
        return this;
    }

    public String toString()
    {
        this.normalize();
        if(this.b.compareTo(BigInteger.ONE) == 0) {
            return this.a + "";
        } else {
            return this.a + "/" + this.b;
        }
    }
}
