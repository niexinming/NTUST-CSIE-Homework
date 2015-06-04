import java.util.*;

public class GenericStack<E> extends ArrayList<E>
{
    public GenericStack()
    {
        super();
    }

    public void push(E obj)
    {
        this.add(obj);
    }

    public E pop()
    {
        int last = this.size() - 1;
        E obj = this.get(last);
        this.remove(last);
        return obj;
    }
}
