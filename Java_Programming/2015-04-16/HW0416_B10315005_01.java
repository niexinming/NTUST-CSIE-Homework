import java.util.*;

public class HW0416_B10315005_01
{
    public static double[][] matrix_mul(double[][] m1, double [][] m2)
    {
        if (m1.length < 1 || m2.length < 1 || m1.length != m2[0].length)
            return null;
        int a = m1.length, b = m2.length, c = m2[0].length;
        // m1(a, b), m2(b, c), r(a, c)

        double result[][] = new double[a][];
        for (int i = 0; i < a; i++) {
            result[i] = new double[c];
            for (int j = 0; j < c; j++) {
                double s = 0;
                for (int l = 0; l < b; l++) {
                    s += m1[i][l] * m2[l][j];
                }
                result[i][j] = s;
            }
        }
        return result;
    }

    public static double[][] read_matrix(int i, int j, Scanner s)
    {
        System.out.printf("Input a %d x %d matrix, seperate by space:\n", i, j);
        double[][] matrix = new double[i][];
        for (int k = 0; k < i; k++) {
            matrix[k] = new double[j];
            for (int l = 0; l < j; l++) {
                matrix[k][l] = s.nextDouble();
            }
        }
        return matrix;
    }

    public static void print_matrix_row(double[][] matrix, int line)
    {
        for (int i = 0; i < matrix[line].length; i++) {
            System.out.printf("%-8.1f", matrix[line][i]);
        }
    }

    public static void print_matrix_multiplication(double[][] m1, double [][] m2, double[][] m3)
    {
        for (int i = 0; i < m1.length; i++) {
            boolean ended = i == m1.length - 1;
            print_matrix_row(m1, i);
            System.out.print(ended ? " * " : "   ");
            print_matrix_row(m2, i);
            System.out.print(ended ? " = " : "   ");
            print_matrix_row(m3, i);
            System.out.println();
        }
    }

    public static void main(String[] args)
    {
        Scanner s = new Scanner(System.in);
        double[][] m1 = read_matrix(2, 2, s);
        double[][] m2 = read_matrix(2, 2, s);
        double[][] result = matrix_mul(m1, m2);
        print_matrix_multiplication(m1, m2, result);
    }
}
