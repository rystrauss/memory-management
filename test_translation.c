#include "translation.h"
#include "frame.h"
#include <stdio.h>

int main() {
    frame_init();
//    for (uint64_t i = 0; i < 256; i++) {
//        if (!vm_map(i, i, 1, 0)) {
//            printf("%llu\n", i);
//        }
//    }

    printf("%d\n", vm_map(134217728, 1, 1, 0));
    printf("%llu\n", vm_translate(0));
    printf("%llu\n", vm_locate(1));

    return 0;
}