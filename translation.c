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
            uint64_t index = ((page + j) >> (3 - i) * 9) & 0b111111111;
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
        uint64_t index = (page + j) & 0b111111111;
        uint64_t offset = (page + j) >> 35;
        (cur + index)->flags = 1;
        (cur + index)->address = (uint64_t) FRAME_ADDRESS(frame + j) + offset;
    }

    return 1;
}

int vm_unmap(uint64_t page, int number) {
    if (!root_table) {
        return 0;
    }
    for (int j = 0; j < number; ++j) {
        entry *cur = root_table;
        for (int i = 0; i < 3; ++i) {
            uint64_t index = ((page + j) >> (3 - i) * 9) & 0b111111111;
            entry *next = cur + index;
            if (!next->flags) {
                return 0;
            }
            cur = (entry *) next->address;
        }
        uint64_t index = (page + j) & 0b111111111;
        (cur + index)->flags = 0;
    }

    return 1;
}

uint64_t vm_locate(int number) {
    if (number < 1 || root_table == NULL) {
        return UINT64_MAX;
    }
    uint64_t start = 0;
    int consecutive = 0;
    uint64_t pages_found = 1;
    for (uint64_t page = 0; page < MAXPAGES; page += pages_found) {
        if (consecutive >= number) {
            return start;
        }
        entry *cur = root_table;
        pages_found = 1;
        for (int i = 0; i < 3; ++i) {
            uint64_t index = (page >> (3 - i) * 9) & 0b111111111;
            entry *next = cur + index;
            if (!next->flags) {
                for (int j = 0; j < 3 - i; ++j) pages_found *= 512;
                break;
            }
            cur = (entry *) next->address;
        }
        uint64_t index = page & 0b111111111;
        if (!(cur + index)->flags || pages_found > 1) {
            if (!consecutive) {
                start = page;
            }
            consecutive += pages_found;
        } else {
            consecutive = 0;
        }
    }
    return UINT64_MAX;
}

uint64_t vm_translate(uint64_t virtual_address) {
    if (!root_table) {
        return UINT64_MAX;
    }
    entry *cur = root_table;
    for (int i = 0; i < 3; ++i) {
        uint64_t index = (virtual_address >> (3 - i) * 9) & 0b111111111;
        entry *next = cur + index;
        if (!next->flags) {
            return UINT64_MAX;
        }
        cur = (entry *) next->address;
    }
    uint64_t index = virtual_address & 0b111111111;
    entry *next = cur + index;
    if (!next->flags) {
        return UINT64_MAX;
    }
    uint64_t offset_mask = (uint64_t) 0b111111111111 << 36;
    return next->address + (virtual_address & offset_mask);
}
