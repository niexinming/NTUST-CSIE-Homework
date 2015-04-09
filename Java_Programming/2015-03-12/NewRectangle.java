public class NewRectangle
{
    double x, y, w, h;

    public NewRectangle(double x, double y, double w, double h)
    {
        this.x = x - w / 2;
        this.y = y - h / 2;
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

    public boolean right_than(NewRectangle r)
    {
        return this.left() >= r.right();
    }

    public boolean left_than(NewRectangle r)
    {
        return r.right_than(this);
    }

    public boolean up_than(NewRectangle r)
    {
        return this.bottom() <= r.top();
    }

    public boolean down_than(NewRectangle r)
    {
        return r.up_than(this);
    }

    public boolean out_of(NewRectangle r)
    {
        return this.right_than(r) || this.left_than(r) ||
            this.up_than(r) || this.down_than(r);
    }

    public boolean in_the(NewRectangle r)
    {
        return this.top() >= r.top() &&
            this.left() >= r.left() &&
            this.right() <= r.right() &&
            this.bottom() <= r.bottom();
    }

    public boolean overlap_with(NewRectangle r)
    {
        return !this.in_the(r) && !this.out_of(r);
    }

    public boolean equals(NewRectangle r)
    {
        return this.x == r.x && this.y == r.y && this.h == r.h && this.w == r.w;
    }
}
