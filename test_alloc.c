#include "translation.h"
#include "frame.h"
#include "kmalloc.h"
#include <stdio.h>

int main() {
    int *p = (int *)kmalloc(4 * sizeof(int));
    printf("%p\n", p);
    kfree(p);
    p = (int *)kmalloc(4 * sizeof(int));
    printf("%p\n", p);
    kfree(p);
    return 0;
}