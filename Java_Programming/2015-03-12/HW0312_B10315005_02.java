import java.util.*;

public class HW0312_B10315005_02
{
    // return a random number in [l, h)
    public static int randint(int l, int h)
    {
        return (int)(Math.random() * (h - l)) + l;
    }

    public static int[] sorted(int[] array)
    {
        Arrays.sort(array);
        return array;
    }

    public static void main(String[] args)
    {
        final int SIZE = 3;

        Scanner scanner = new Scanner(System.in);

        int[] arr = new int[SIZE];
        int[] inp = new int[SIZE];
        int[] map = new int[10]; // 0 ~ 9

        System.out.print("[WARNING] D0 N0T PEEKING: ");
        for (int i = 0; i < SIZE; i++) {
            arr[i] = randint(0, 10);
            map[arr[i]]++;
            System.out.print(arr[i]);
        }
        System.out.println();

        System.out.print("Input three digit-number: ");
        String input = scanner.nextLine();
        if (input.length() < SIZE) {
            System.out.println("Not enough length!");
            return;
        }
        for (int i = 0; i < SIZE; i++) {
            int val = input.charAt(i) - '0';
            if (0 <= val && val <= 9) {
                inp[i] = val;
            } else {
                System.out.println("Invalid character: " + input.charAt(i));
                return;
            }
        }

        if (Arrays.equals(arr, inp)) {
            System.out.println("You Win!!!! $12,000!!");
        } else if (Arrays.equals(sorted(arr), sorted(inp))) {
            System.out.println("You Win $3,000!");
        } else {
            int match = 0;

            for (int i = 0; i < SIZE; i++) {
                if (map[inp[i]] > 0) {
                    map[inp[i]]--;
                    match++;
                }
            }
            System.out.printf("You Win %,d$!\n", match * 1000);
        }
    }
}
