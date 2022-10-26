#include <stdio.h>

int main() {
    int x;
    printf("Enter an integer: ");
    scanf("%d", &x);
    return !(x % 400) || ((x % 100) && !(x%4));
}