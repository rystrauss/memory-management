#include "translation.h"
#include "frame.h"
#include <stdio.h>

int main() {
    frame_init();
    for (uint64_t i = 0; i < 513; i++) {
        if (!vm_map(i, i, 1, 0)) {
            printf("%llu\n", i);
        }
    }

    printf("%llu\n", vm_locate(2));
    printf("%llu\n", vm_translate(2));
    printf("%d\n",  vm_unmap(2, 10));
    printf("%llu\n", vm_translate(2));
    printf("%llu\n", vm_locate(2));
    printf("%llu\n", vm_locate(11));
    printf("%d\n",  vm_map(2, 1, 8, 0));
    printf("%llu\n", vm_locate(2));

    return 0;
}