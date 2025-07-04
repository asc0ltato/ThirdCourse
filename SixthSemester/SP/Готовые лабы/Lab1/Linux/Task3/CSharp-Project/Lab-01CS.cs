using System;
using System.Text;

class Program
{
    static int ReverseNumber(int x)
    {
        int hundreds = x / 100;
        int tens = (x / 10) % 10;
        int ones = x % 10;
        return ones * 100 + tens * 10 + hundreds;
    }

    static void Main()
    {
        Console.Write("Enter a three-digit number: ");
        int number = int.Parse(Console.ReadLine());

        if (number < 100 || number > 999)
        {
            Console.WriteLine("Error: enter a three-digit number!");
            Console.WriteLine("Press Enter to exit.");
            Console.ReadLine();
            return;
        }

        int reversed = ReverseNumber(number);

        Console.WriteLine("The number is in reverse order: " + reversed);
        Console.WriteLine("Press Enter to exit.");
        Console.ReadLine();
    }
}