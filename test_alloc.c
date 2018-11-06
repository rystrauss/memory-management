#include "translation.h"
#include "frame.h"
#include "kmalloc.h"
#include <stdio.h>

int main() {
    frame_init();
    for (uint64_t i = 0; i < 513; i++) {
        if (!vm_map(i, i, 1, 0)) {
            printf("%llu\n", i);
        }
    }
    palloc(4);
    return 0;
}