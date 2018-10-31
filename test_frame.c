#include "frame.h"
#include <stdio.h>

int main() {
    frame_init();
    int64_t a = allocate_frame(1024);
    int64_t b = deallocate_frame(40, 200);
    int64_t c = allocate_frame(120);
    int64_t d = allocate_frame(90);
    printf("%lld, %lld, %lld, %lld\n", a, b, c, d);
    return 0;
}