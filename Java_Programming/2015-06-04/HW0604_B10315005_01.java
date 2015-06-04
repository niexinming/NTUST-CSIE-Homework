import java.util.*;

public class HW0604_B10315005_01
{
    public static void main(String[] args)
    {
        GenericStack<Integer> stack = new GenericStack<Integer>();

        stack.push(1);
        stack.push(2);
        stack.push(3);
        System.out.println("pop -> " + stack.pop());
        stack.push(4);
        System.out.println("pop -> " + stack.pop());
        System.out.println("pop -> " + stack.pop());
        System.out.println("pop -> " + stack.pop());
    }
}
