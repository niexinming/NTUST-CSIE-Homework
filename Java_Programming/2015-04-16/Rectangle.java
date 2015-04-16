public class Rectangle
{
    public double width, height;

    public Rectangle()
    {
        this.width = 0;
        this.height = 0;
    }

    public Rectangle(double w, double h)
    {
        this.width = w;
        this.height = h;
    }

    public double getArea()
    {
        return this.width * this.height;
    }

    public double getPerimeter()
    {
        return 2 * (this.width + this.height);
    }

    public void selfTest()
    {
        System.out.printf("This is a Rectangle object with width: %f and height: %f\n", this.width,
                          this.height);
        System.out.printf("getArea() = %f, getPerimeter() = %f\n", this.getArea(),
                          this.getPerimeter());
        System.out.println("selfTest() done!");
    }
}
