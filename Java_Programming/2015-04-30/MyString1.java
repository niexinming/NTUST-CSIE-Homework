public class MyString1
{
    private char[] buffer;

    public static MyString1 valueOf(int i)
    {
        return new MyString1(("" + i).toCharArray());
    }

    public MyString1(char[] data)
    {
        this.buffer = data;
    }

    public char charAt(int index)
    {
        return this.buffer[index];
    }

    public int length()
    {
        return this.buffer.length;
    }

    public MyString1 substring(int begin, int end)
    {
        int length = end - begin;
        char[] new_buffer = new char[length];
        System.arraycopy(this.buffer, begin, new_buffer, 0, length);
        return new MyString1(new_buffer);
    }

    public MyString1 toLowerCase()
    {
        char[] new_buffer = new char[this.length()];
        for(int i = 0; i < new_buffer.length; i++) {
            char ch = this.buffer[i];
            if ('A' <= ch && ch <= 'Z')
                ch = (char)((int)ch + 0x20);
            new_buffer[i] = ch;
        }
        return new MyString1(new_buffer);
    }

    public char[] toCharArray()
    {
        return this.buffer;
    }

    public boolean equals(MyString1 s)
    {
        if (s.length() != this.length())
            return false;
        char[] b = s.toCharArray();
        for (int i = 0; i < b.length; i++) {
            if (b[i] != this.buffer[i]) {
                return false;
            }
        }
        return true;
    }

    public String toString()
    {
        return new String(this.buffer);
    }
}
