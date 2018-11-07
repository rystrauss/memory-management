#include "kmalloc.h"
#include "translation.h"
#include "frame.h"
#include "ll_double.h"

// After you are done, you don't need to import it anymore. This is here just for the dummy code on malloc/free.
#include<stdlib.h>
#include <stdio.h>

#define MAGIC_NUMBER 0xc0dec0de

struct list *free_list = NULL;

void *palloc(uint64_t number) {
    // 1) Allocate <parameter:number> frames of memory
    int64_t frame_number = allocate_frame((int) number);
    // 2) Find the first page number (virtual) that is not mapped to a frame (physical), and <parameter:number> of them are consecutive
    uint64_t page_number = vm_locate((int) number);
    // 3) IGNORE the result of the previous call, and map the frame number to itself
    page_number = (uint64_t) frame_number;
    vm_map(page_number, (uint64_t) frame_number, (int) number, 0);
    // 4) Return the address of the first byte of the allocated page [see note below]
    return PAGE_ADDRESS(page_number);

    // NOTE:
    // - You are simulating the hardware automatic address translation in software.
    // - The palloc() function would return the address of the first virtual (mapped) address, based on the page number of step (2),
    //   but your application would not understand this since it only understands its own application addressing.
    // - Therefore, you are going to SIMULATE that there is another layer of automatic virtual addressing by
    //   always mapping page number X to the address of a frame number X
    // - Inside a real kernel, you wouldn't perform the overwrite of the page number in step (3),
    //   but the hardware would be performing the automatic translation between page_number to frame_number

    // Make sure, that in the end of this function:
    // vm_translate(page) is FRAME_ADDRESS(frame)

}

void pfree(void *address, uint64_t number) {
    // 1) Unmap the page number starting at the provided address (the address is the first byte of a page)
    //    This means:
    // 2) Do that for the number of times provided in the parameter number

    uint64_t page = (uint64_t) PAGE_NUMBER((char *) address);

    uint64_t frame = vm_translate(page);

    vm_unmap(page, number);
    deallocate_frame((uint64_t) FRAME_NUMBER((char *) frame), number);
}

void *kmalloc(uint64_t size) {
    // - Implement a linked list of free chunks using only palloc() [see notes below]
    // - Use the first-fit strategy to allocate a chunk
    if (!free_list) {
        ll_init(free_list);
        uint64_t *address = palloc(1);
        *address = 4096 - 2 * sizeof(uint64_t);
        *(address + 1) = 0;
        ll_insert_head(free_list, address);
    }
    struct node *cur = free_list->head;
    while (!cur) {
        if (*((uint64_t *) cur->data + 1) != MAGIC_NUMBER) {

        }
        cur = *((uint64_t *) cur->data + 1);
    }
    return malloc(size);
}

void *krealloc(void *address, uint64_t size) {
    // TODO:
    // - If the address is becoming smaller, return the last frames that have become unused with vm_unmap() and frame_deallocate()
    // - If the address is becoming bigger, and is possible to allocate new contiguous pages to extend the chunk size,
    //   allocate new frames and self-map their numbers to page numbers (see notes on the palloc() function for a discussion of self-mapping)
    // - If the address is becomming bigger, but it is not possible to allocate new contigous pages to extend the chunk size,
    //   use kmallloc() to allocate a new chunk, then use memcpy() to copy the original bytes to the new chunk, and return the new chunk's address.
    //   Before returning, free the old chunk (by merging their free space to another header)

    // Dummy code: you cannot use malloc/free/realloc
    return realloc(address, size);
}

void kfree(void *address) {
    // TODO:
    // - Make the space used by the address free
    // - Return any frames that have become unused with vm_unmap() and frame_deallocate()

    // Dummy code: you cannot use malloc/free/realloc
    return free(address);
}
