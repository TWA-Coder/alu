#include <stdio.h>

int max(int , int);

int main() {
    int a = 10;
    int b = 20;
    printf("The maximum of %d and %d is %d\n", a, b, max(a, b));
    return 0;
}

int max(int x, int y) {
    return (x > y) ? x : y;
}
