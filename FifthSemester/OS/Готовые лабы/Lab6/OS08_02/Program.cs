using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Threading;
using System.Threading.Tasks;

class Program
{
    static void Main(string[] args)
    {
        List<Big> bigObjects = new List<Big>();

        while (true)
        {
            Big bigObject = new Big();
            bigObjects.Add(bigObject);

            Task.Run(() => bigObject.FillArray());

            long memoryUsed = GC.GetTotalMemory(true);
            Console.WriteLine("Используемая память: {0} MB", (memoryUsed / (1024 * 1024)));

            Thread.Sleep(1000);
        }
    }
}

class Big
{
    public Int32[] IntArray;

    public Big()
    {
        IntArray = new int[128 * 1024 * 1024 / sizeof(int)];
    }

    public void FillArray()
    {
        Random rand = new Random();
        for (int i = 0; i < IntArray.Length; i++)
        {
            IntArray[i] = rand.Next();
        }
    }
}

/*class Program
{
    static void Main(string[] args)
    {
        int mem = 0; // грубая оценка, нужно спросить у системы
        List<Big> lbig = new List<Big>(1000);
        while (true)
        {
            lbig.Add(new Big()); mem += 1048576 * 128;
            Console.WriteLine("{0,-6} MB", (mem / 1048576)); Thread.Sleep(5000);
        }
    }
}

class Big
{
    public Int32[] IntArray; public Big()
    {
        IntArray = new int[1048576 * 32];
    }
}*/