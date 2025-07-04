class Program
{
    static int Count = 0; 

    static void WorkThread()
    {
        for (int i = 0; i < 5000000; ++i)
        {
            Count = Count + 1;
        }
    }

    static void Main(string[] args)
    {
        Thread[] t = new Thread[20];
        for (int i = 0; i < 20; ++i)
        {
            t[i] = new Thread(WorkThread);
            t[i].Start(); 
        }
        for (int i = 0; i < 20; ++i)
        {
            t[i].Join(); 
        }

        int num = 20 * 5000000;
        Console.WriteLine($"Результат Count: {Count}");
        Console.WriteLine($"Ожидаемое значение (20 * 5000000): {num}");

        if (Count < num)
        {
            double ratio = (double)num / Count;
            Console.WriteLine($"Результат меньше ожидаемого в {ratio:F2} раз");
        }
        else if (Count > num)
        {
            double ratio = (double)Count / num;
            Console.WriteLine($"Результат больше ожидаемого в {ratio:F2} раз");
        }
        else
        {
            Console.WriteLine("Результат совпадает с ожидаемым значением");
        }
    }
}