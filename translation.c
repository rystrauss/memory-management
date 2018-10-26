#include<stdio.h>

#include "translation.h"
#include "frame.h"

void *root_table = NULL;

int vm_map(uint64_t page, uint64_t frame, int number, uint16_t flags) {
    if (root_table == NULL) {
        int64_t frame_number = allocate_frame(1);
        char *frame_address = FRAME_ADDRESS(frame_number);
        entry *table = (entry *) frame_address;
        root_table = table;

        for (int i = 0; i < 4096; i++) {
            root_table + i = NULL;
        }
    }
    entry *cur = root_table;
    for (int i = 0; i < 3; ++i) {
        uint64_t index = (page << (12 + i * 9)) >> ((3 - i) * 9 + 12);

        if (cur == NULL) {
            int64_t frame_number = allocate_frame(1);
            char *frame_address = FRAME_ADDRESS(frame_number);
            entry *table = (entry *) frame_address;
            cur = table;
        } else {

        }

    }
    return -1;
}

int vm_unmap(uint64_t page, int number) {
    return -1;
}

uint64_t vm_locate(int number) {
    return -1;
}

uint64_t vm_translate(uint64_t virtual_address) {
    // Dummy code: there's no translation being done
    return virtual_address;
}
