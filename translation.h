#ifndef TRANSLATION_H
#define TRANSLATION_H

#include<stdint.h>

struct entry_t_ {
    /// 52-bit can be used to put the address of the frame
    uint64_t address : 52;
    // Unused (3 bits)
    uint16_t unused : 3;
    /// Page flags (9 bits)
    uint16_t flags : 9;
};

typedef struct entry_t_ entry;

/// 4-level page table implementing the translation from virtual to physical
extern void *table_root;

/**
 * 1. We multiply by 4096 because that is the size of a frame, so doing this will result in the actual memory address
 *    of the given frame.
 * 2. We add the base memory address because our memory does not start at zero, like what would happen in an
 *    actual kernel.
 * 3. In a real kernel, the program has access to the computer's entire memory, which begins at address zero, so
 *    summing the base address is actually just adding zero.
 */

/**
 * 1. We divide by 4096 because that is the size of a frame, so doing this will result in the frame number
 *    of the given address.
 * 2. We subtract the base memory address because our memory does not start at zero, like what would happen in an
 *    actual kernel.
 * 3. In a real kernel, the program has access to the computer's entire memory, which begins at address zero, so
 *    subtracting the base address is actually just subtracting zero.
 */
#define PAGE_NUMBER(address) ((address - memory) / 4096)
#define PAGE_ADDRESS(frame) ((memory + (frame * 4096)))

/**
 * Maps @p number pages starting at page number @p page to consecutive frames starting at frame number @p frame,
 * each page mapped using the flags provided in @p flags.
 *
 * This function dynamically grows the table tree. Whenever a new table needs to be created, the function allocate_frame
 * is used to get a new frame containing the entries of the table.
 *
 * @param page First page that will be mapped.
 * @param frame First frame that pages map into.
 * @param number Number of consecutive virtual pages mapped into physical frames.
 * @param flags Flags used in each mapping.
 *
 * @return A positive number on success, and 0 on failure.
 */
int vm_map(uint64_t page, uint64_t frame, int number, uint16_t flags);

/**
 * Unmaps @p number pages starting at page number @p page.
 *
 * @param page First page that will be mapped.
 * @param number Number of consecutive virtual pages mapped into physical frames.
 *
 * @return A positive number on success, and 0 on failure.
 */
int vm_unmap(uint64_t page, int number);

/**
 * Locates the first @p number unmapped pages starting at page number @p page_low.
 *
 *
 * @param page_low Page from which start our search.
 *
 * @return The first page number of a sequence of @p number pages that are not mapped.
 */
uint64_t vm_locate(int number);

/**
 * Returns the physical address associated with virtual address @p virtual_address.
 *
 * @param virtual_addresss Address that will be translated into a physical address.
 *
 * @return The physical address associated with virtual address @p virtual_address.
 */
uint64_t vm_translate(uint64_t virtual_address);

#endif /* TRANSLATION_H */
