#include<stdio.h>

#include "translation.h"
#include "frame.h"

#define MAXPAGES 281474976710656

void *root_table = NULL;

entry *make_table() {
    int64_t frame_number = allocate_frame(1);
    if (frame_number == -1)
        return NULL;
    uint64_t frame_address = (uint64_t) FRAME_ADDRESS(frame_number);
    entry *table = (entry *) frame_address;

    for (int i = 0; i < 512; i++) {
        *(table + i) = (entry) {.address = 0, .flags = 0, .unused = 0};
    }

    return table;
}

int vm_map(uint64_t page, uint64_t frame, int number, uint16_t flags) {
    if (!root_table) {
        root_table = make_table();
        if (!root_table) {
            return 0;
        }
    }
    for (int j = 0; j < number; ++j) {
        entry *cur = root_table;
        for (int i = 0; i < 3; ++i) {
            uint64_t index = ((page + j) << (12 + i * 9)) >> ((12 + i * 9) + (3 - i) * 9);
            entry *next = cur + index;
            if (!next->flags) {
                entry *new_table = make_table();
                if (!new_table) {
                    return 0;
                }
                next->address = (uint64_t) new_table;
                next->flags = 1;
            }
            cur = (entry *) next->address;
        }
        uint64_t index = ((page + j) << (12 + 3 * 9)) >> (12 + 3 * 9);
        uint64_t offset = (page + j) >> 35;
        (cur + index)->flags = 1;
        (cur + index)->address = (uint64_t) FRAME_ADDRESS(frame + j) + offset;
    }

    return 1;
}

int vm_unmap(uint64_t page, int number) {
    entry *cur = root_table;
    if (!cur) {
        return 0;
    }
    for (int j = 0; j < number; ++j) {
        for (int i = 0; i < 3; ++i) {
            uint64_t index = ((page + j) << (12 + i * 9)) >> ((12 + i * 9) + (3 - i) * 9);
            entry *next = cur + index;
            if (!next->flags) {
                return 0;
            }
            cur = (entry *) next->address;
        }
        uint64_t index = ((page + j) << (12 + 3 * 9)) >> (12 + 3 * 9);
        (cur + index)->flags = 0;
    }

    return 1;
}

uint64_t vm_locate(int number) {
    if (number < 1)
        return UINT64_MAX;
    uint64_t start = 0;
    int consecutive = 0;
    for (uint64_t page = 0; page < MAXPAGES; ++page) {
        if (consecutive == number) {
            return start;
        }
        entry *cur = root_table;
        for (int i = 0; i < 3; ++i) {
            uint64_t index = ((page) << (12 + i * 9)) >> ((12 + i * 9) + (3 - i) * 9);
            entry *next = cur + index;
            if (next->flags) {
                consecutive = 0;
                break;
            }
            cur = (entry *) next->address;
        }
        if (!cur->flags) {
            if (!consecutive) {
                start = page;
            }
            consecutive++;
        }
    }
    return UINT64_MAX;
}

uint64_t vm_translate(uint64_t virtual_address) {
    entry *cur = root_table;
    for (int i = 0; i < 3; ++i) {
        uint64_t index = ((virtual_address) << (12 + i * 9)) >> ((12 + i * 9) + (3 - i) * 9);
        entry *next = cur + index;
        if (!next->flags) {
            return UINT64_MAX;
        }
        cur = (entry *) next->address;
    }
    return cur->address;
}
