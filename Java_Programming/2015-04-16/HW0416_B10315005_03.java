public class HW0416_B10315005_03
{
    public static boolean greater(double[] a, double b[])
    {
        if (a[0] > b[0]) return true;
        else if (a[0] == b[0]) return a[1] > b[1];
        else return false;
    }

    public static double[][] merge_sort(double[][] data, int left, int right)
    {
        int l = right - left;
        if (l == 1) {
            return new double[][] { data[left] };
        }
        double[][] left_arr = merge_sort(data, left, left + l / 2);
        double[][] right_arr = merge_sort(data, left + l / 2, right);
        double[][] result = new double[l][];
        int left_p = 0, right_p = 0, result_p = 0;

        while (left_p < left_arr.length && right_p < right_arr.length) {
            if (greater(left_arr[left_p], right_arr[right_p])) {
                result[result_p++] = right_arr[right_p++];
            } else {
                result[result_p++] = left_arr[left_p++];
            }
        }
        while (left_p < left_arr.length) {
            result[result_p++] = left_arr[left_p++];
        }
        while (right_p < right_arr.length) {
            result[result_p++] = right_arr[right_p++];
        }
        return result;
    }

    public static void print_row(double[][] row)
    {
        for (int i = 0; i < row.length; i++) {
            System.out.printf("{ %.0f, %.0f }", row[i][0], row[i][1]);
            if (i != row.length - 1) System.out.print(", ");
            else System.out.println();
        }
    }

    public static void main(String[] args)
    {
        double[][][] test_data = {
            { { 9, 7 }, { 7, 6 }, { 0, 2 }, { 6, 9 }, { 9, 0 }, { 2, 3 }, { 6, 3 }, { 9, 6 } },
            { { 0, 4 }, { 2, 2 }, { 9, 7 }, { 9, 5 }, { 4, 1 }, { 9, 3 }, { 9, 2 }, { 6, 5 } },
            { { 5, 4 }, { 4, 4 }, { 6, 9 }, { 9, 8 }, { 9, 0 }, { 2, 8 }, { 9, 5 }, { 1, 1 } },
            { { 5, 4 }, { 4, 7 }, { 2, 8 }, { 8, 8 }, { 8, 8 }, { 1, 0 }, { 1, 8 }, { 9, 6 } },
            { { 1, 9 }, { 0, 2 }, { 3, 6 }, { 9, 4 }, { 9, 1 }, { 3, 4 }, { 4, 9 }, { 2, 8 } },
            { { 7, 0 }, { 5, 5 }, { 3, 8 }, { 8, 0 }, { 5, 1 }, { 7, 6 }, { 6, 1 }, { 9, 2 } },
            { { 8, 5 }, { 4, 3 }, { 5, 7 }, { 4, 9 }, { 5, 5 }, { 4, 1 }, { 9, 3 }, { 2, 1 } },
            { { 6, 6 }, { 9, 5 }, { 8, 2 }, { 7, 6 }, { 7, 2 }, { 0, 0 }, { 9, 2 }, { 1, 3 } }
        };

        for (int i = 0; i < test_data.length; i++) {
            System.out.print("Before: ");
            print_row(test_data[i]);
            double[][] sorted = merge_sort(test_data[i], 0, test_data[i].length);
            System.out.print("After : ");
            print_row(sorted);
            System.out.println();
        }
    }
}
