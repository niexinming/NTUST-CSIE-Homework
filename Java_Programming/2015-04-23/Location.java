public class Location
{
    public int column;
    public int row;
    public double minValue;

    public Location(int row, int column, double minValue)
    {
        this.row = row;
        this.column = column;
        this.minValue = minValue;
    }

    public static Location localeSmallest(double[][] a)
    {
        Location l = new Location(0, 0, a[0][0]);
        for (int r = 0; r < a.length; r++) {
            for (int c = 0; c < a[r].length; c++) {
                if (a[r][c] < l.minValue) {
                    l.row = r;
                    l.column = c;
                    l.minValue = a[r][c];
                }
            }
        }
        return l;
    }
}
