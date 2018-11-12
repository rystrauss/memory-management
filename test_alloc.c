#include "translation.h"
#include "frame.h"
#include "kmalloc.h"
#include <stdio.h>

int main() {
    int *a = (int *)kmalloc(4 * sizeof(int));
    int *b = (int *)kmalloc(4 * sizeof(int));
    int *c = (int *)kmalloc(4 * sizeof(int));
    int *d = (int *)kmalloc(4 * sizeof(int));
    kfree(b);
    kfree(c);
    printf("%p\n", a);
    return 0;
}