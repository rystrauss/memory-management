#include <string.h>

#include "kmalloc.h"
#include "translation.h"
#include "frame.h"

// After you are done, you don't need to import it anymore. This is here just for the dummy code on malloc/free.
#include<stdlib.h>
#include <stdio.h>

#define MAGIC_NUMBER 0xc0dec0de
#define HEADER_SIZE (2 * sizeof(uint64_t))
#define THRESHOLD (2 * HEADER_SIZE)

uint64_t *head = (uint64_t *) UINT64_MAX;

void *palloc(uint64_t number) {
    // 1) Allocate <parameter:number> frames of memory
    int64_t frame_number = allocate_frame((int) number);
    if (frame_number == -1) {
        return (void *) UINT64_MAX;
    }
    // 2) Find the first page number (virtual) that is not mapped to a frame (physical), and <parameter:number> of them are consecutive
    uint64_t page_number = vm_locate((int) number);
    // 3) IGNORE the result of the previous call, and map the frame number to itself
    page_number = (uint64_t) frame_number;
    if (!vm_map(page_number, (uint64_t) frame_number, (int) number, 0)) {
        return (void *) UINT64_MAX;
    }
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

    vm_unmap(page, (int) number);
    deallocate_frame((uint64_t) FRAME_NUMBER((char *) frame), (int) number);
}

void *kmalloc(uint64_t size) {
    // - Implement a linked list of free chunks using only palloc() [see notes below]
    // - Use the first-fit strategy to allocate a chunk
    if (head == (uint64_t *) UINT64_MAX) {
        uint64_t *address = (uint64_t *) palloc(1);
        if (address == (uint64_t *) UINT64_MAX) {
            return 0;
        }
        *address = 4096 - 2 * sizeof(uint64_t);
        *(address + 1) = 0;
        head = address;
    }
    uint64_t *prev = (uint64_t *) UINT64_MAX;
    uint64_t *cur = head;
    while (1) {
        // Found space
        if (*cur >= size) {
            break;
        }
        // At the tail, and have not found free space
        // Need to palloc a new frame
        if (!*(cur + 1)) {
            uint64_t num_frames = (size / 4096) + ((size % 4096) ? 1 : 0);
            uint64_t *address = (uint64_t *) palloc(num_frames);
            if (address == (uint64_t *) UINT64_MAX) {
                return 0;
            }
            *address = (4096 * num_frames) - (2 * sizeof(uint64_t));
            *(address + 1) = 0;
            *(cur + 1) = (uint64_t) address;
            prev = cur;
            cur = address;
        }
            // Check the next free space
        else {
            prev = cur;
            cur = (uint64_t *) *(cur + 1);
        }
    }
    // At this point, cur is a suitable location to put our stuff
    if (cur == head) {
        // If we are using 'all' of the free space
        if (*cur - size <= THRESHOLD) {
            // If head is only thing, set head to null, otherwise set to next node
            head = !*(cur + 1) ? (uint64_t *) UINT64_MAX : (uint64_t *) *(cur + 1);
            *(cur + 1) = MAGIC_NUMBER;
            return cur + 2;
        }
            // Part of the head
        else {
            uint64_t *next = (uint64_t *) *(cur + 1);
            uint64_t free_space = *cur;
            *cur = size;
            *(cur + 1) = MAGIC_NUMBER;
            head = (uint64_t *) (((char *) cur) + size + HEADER_SIZE);
            *head = free_space - size - HEADER_SIZE;
            *(head + 1) = (uint64_t) next;
            return cur + 2;
        }
    }
        // If not the head
    else {
        // If we are using 'all' of the free space
        if (*cur - size <= THRESHOLD) {
            *(prev + 1) = *(cur + 1);
            *(cur + 1) = MAGIC_NUMBER;
            return cur + 2;
        }
            // Part of the node
        else {
            uint64_t *next = (uint64_t *) *(cur + 1);
            uint64_t free_space = *cur;
            *cur = size;
            *(cur + 1) = MAGIC_NUMBER;
            cur = (uint64_t *) (((char *) cur) + size + HEADER_SIZE);
            *cur = free_space - size - HEADER_SIZE;
            *(cur + 1) = (uint64_t) next;
            *(prev + 1) = (uint64_t) cur;
            return cur + 2;
        }
    }
}

void *krealloc(void *address, uint64_t size) {
    // TODO:
    // - If the address is becoming smaller, return the last frames that have become unused with vm_unmap() and frame_deallocate()
    // - If the address is becoming bigger, and is possible to allocate new contiguous pages to extend the chunk size,
    //   allocate new frames and self-map their numbers to page numbers (see notes on the palloc() function for a discussion of self-mapping)
    // - If the address is becomming bigger, but it is not possible to allocate new contigous pages to extend the chunk size,
    //   use kmallloc() to allocate a new chunk, then use memcpy() to copy the original bytes to the new chunk, and return the new chunk's address.
    //   Before returning, free the old chunk (by merging their free space to another header)

    // Need to move to a new chunk of memory
    if (*((uint64_t *) address) < size) {
        uint64_t *new_location = kmalloc(size);
        memcpy(new_location, address, *((uint64_t *) address));
        kfree(address);
        return (void *) (new_location + 2);
    } else {
        uint64_t primordial_size = *((uint64_t *) address); // The size of the original node
        if (primordial_size - size > THRESHOLD) {
            *((uint64_t *) (address - 2)) = size;
            *((uint64_t *) (address + size + sizeof(uint64_t))) = (uint64_t) head;
            head = (uint64_t *) (address + size);
        }
        return address;
    }
}

void kfree(void *address) {
    // - Make the space used by the address free
    // - Return any frames that have become unused with vm_unmap() and frame_deallocate()
    uint64_t *cur = head;
    while (*(cur + 1)) {
        // If the current node is right before the address
        if (cur + *cur + HEADER_SIZE == address - HEADER_SIZE) {
            *cur += *((uint64_t *) (address - 2)) + HEADER_SIZE;
            if (cur + *cur == (uint64_t *) *(cur + 1)) {
                *cur += *((uint64_t *) *(cur + 1));
                *(cur + 1) = *((uint64_t *) *(cur + 1) + 1);
            }
            return;
        }
        // If the current node is right after the address
        if (cur == address + *((uint64_t *) address - 2)) {
            *((uint64_t *) address - 2) += *cur + HEADER_SIZE;
            *((uint64_t *) address - 1) = *(cur + 1);
            return;
        }
        cur = (uint64_t *) *(cur + 1);
    }
    *((uint64_t *) (address - 1)) = (uint64_t) head;
    head = (uint64_t *) (address - 2);
}
