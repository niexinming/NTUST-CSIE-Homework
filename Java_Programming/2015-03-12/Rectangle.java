public class Rectangle
{
    double x, y, w, h;

    public Rectangle(double x, double y, double w, double h)
    {
        this.x = x;
        this.y = y;
        this.w = w;
        this.h = h;
    }

    public double top()
    {
        return this.y;
    }

    public double left()
    {
        return this.x;
    }

    public double bottom()
    {
        return this.y + this.h;
    }

    public double right()
    {
        return this.x + this.w;
    }

    public boolean right_than(Rectangle r)
    {
        return this.left() >= r.right();
    }

    public boolean left_than(Rectangle r)
    {
        return r.right_than(this);
    }

    public boolean up_than(Rectangle r)
    {
        return this.bottom() <= r.top();
    }

    public boolean down_than(Rectangle r)
    {
        return r.up_than(this);
    }

    public boolean out_of(Rectangle r)
    {
        return this.right_than(r) || this.left_than(r) ||
            this.up_than(r) || this.down_than(r);
    }

    public boolean in_the(Rectangle r)
    {
        return this.top() >= r.top() &&
            this.left() >= r.left() &&
            this.right() <= r.right() &&
            this.bottom() <= r.bottom();
    }

    public boolean overlap_with(Rectangle r)
    {
        return !this.in_the(r) && !this.out_of(r);
    }

    public boolean equals(Rectangle r)
    {
        return this.x == r.x && this.y == r.y && this.h == r.h && this.w == r.w;
    }
}
