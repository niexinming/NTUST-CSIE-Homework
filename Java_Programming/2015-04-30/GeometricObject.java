public class GeometricObject
{
    // Variables
    private boolean filled;
    private String color;

    // Getters
    public boolean getFilled() { return this.filled; }
    public String getColor() { return this.color; }

    // Setters
    public void setFilled(boolean filled) { this.filled = filled; }
    public void setColor(String color) { this.color = color; }

    public String toString()
    {
        return "GeometricObject color = " + this.color + " filled = " + this.filled;
    }
}
