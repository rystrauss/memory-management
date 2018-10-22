#include<stdio.h>

#include "translation.h"

void *table_root = NULL;

int vm_map(uint64_t page, uint64_t frame, int number, uint16_t flags) {
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
