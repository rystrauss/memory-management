#include "translation.h"
#include "frame.h"
#include "kmalloc.h"
#include <stdio.h>

int main() {
    frame_init();
    void* address = palloc(4);
    pfree(address + 1, 2);
    return 0;
}