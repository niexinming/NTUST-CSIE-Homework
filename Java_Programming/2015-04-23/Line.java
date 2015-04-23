public class Line
{
    public double m;
    public double c;
    public boolean isVertical = false;
    public Point p1, p2;

    public Line(Point p1, Point p2)
    {
        this.p1 = p1;
        this.p2 = p2;

        if (p1.x == p2.x) {
            this.isVertical = true;
            this.m = 0;
            this.c = p1.x;
        } else {
            this.m = (p1.y - p2.y) / (p1.x - p2.x);
            this.c = p1.y - p1.x * this.m;
        }
    }

    public double f(double x, double y)
    {
        // calculate: f(x, y) = m * x - y + c
        return this.m * x - y + this.c;
    }

    public double f(Point p)
    {
        return this.f(p.x, p.y);
    }

    public boolean isIntersectWith(Line l)
    {
        return l.f(this.p1) * l.f(this.p2) < 0 && this.f(l.p1) * this.f(l.p2) < 0;
    }

    public Point calculateIntersectPoint(Line l)
    {
        return new Point(
                (l.c - this.c) / (this.m - l.m),
                (this.m * l.c - l.m * this.c) / (this.m - l.m)
                );
    }
}
