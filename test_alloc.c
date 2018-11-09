#include "translation.h"
#include "frame.h"
#include "kmalloc.h"
#include <stdio.h>

int main() {
    frame_init();
    for (int i = 0; i < 10; ++i) {
        vm_map(i, i, 1, 0);
    }
    void* address = palloc(4);
    pfree(address + 1, 2);
    return 0;
}