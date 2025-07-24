#include <iostream>

unsigned fact(unsigned x) {
    if (x < 2) return 1;
    return x * fact(x - 1);
}

int main() {
    unsigned f = fact(5);
    printf ("u\n", f);
}