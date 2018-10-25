#include "frame.h"
#include <stdio.h>

int main() {
    frame_init();
    int64_t allocate = allocate_frame(1024);
    int64_t deallocate = deallocate_frame(0, 1024);
    printf("%lld, %lld\n", allocate, deallocate);
    return 0;
}