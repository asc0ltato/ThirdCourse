#include <stdio.h>

int reverseNumber(int x) {
    int hundreds = x / 100;
    int tens = (x / 10) % 10;
    int ones = x % 10;
    return ones * 100 + tens * 10 + hundreds;
}

int main() {
    int number;
    printf("Enter a three-digit number: ");
    scanf("%d", &number);
    getchar();

    if (number < 100 || number > 999) {
        printf("Error: enter a three-digit number!\n");
        printf("Press Enter to exit.");
        getchar();
        return 1;
    }

    int reversed = reverseNumber(number);

    printf("The number is in reverse order: %d\n", reversed);
    printf("Press Enter to exit.");
    getchar();
    return 0;
}