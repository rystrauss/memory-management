/**
 * Functions for handling frame allocation.
 *
 * @author Ryan Strauss
 * @author Niall Williams
 */

#include "frame.h"

#define NUM_FRAMES 1024
#define BITMAP_LENGTH (NUM_FRAMES / 64)
#define BIT_VALUE(ll, i) ((ll << (63 - i)) >> 63)

uint64_t frames_allocated;

uint64_t frames_available;

// No need to change or initialize this. This is "empty" space that simulate your physical memory.
_Alignas(4096) char memory[MEMORY_SIZE];

// You should define a bitmap to keep track of the allocated frames here.
// Look at the handout for details on size
uint64_t bitmap[BITMAP_LENGTH];

void bit_on(uint64_t *ll, int i) {
    *ll |= 1ULL << i;
}

void bit_off(uint64_t *ll, uint64_t i) {
    *ll &= ~(1ULL << i & *ll);
}

void frame_init() {
    // Initialize global variables
    // Initialize the bitmap
    frames_allocated = 0;
    frames_available = NUM_FRAMES;

    for (int i = 0; i < BITMAP_LENGTH; ++i) {
        bitmap[i] = 0;
    }
}

int64_t allocate_frame(int number_frames) {
    // Consult the bitmap and return the first available frame number, marking it as allocated
    // Increase the frames_allocated, decrease frames_available
    if (!frames_available)
        return -1;
    int index = 0, start = 0;
    while (index <= NUM_FRAMES) {
        if (index - start == number_frames) {
            frames_allocated += number_frames;
            frames_available -= number_frames;
            for (int i = start; i < index; ++i) {
                bit_on(&bitmap[i / 64], i % 64);
            }
            return start;
        }
        if (BIT_VALUE(bitmap[index / 64], index % 64))
            start = index + 1;
        index++;
    }
    return -1; // Return according to what's documented in the header file for this module
}

int64_t deallocate_frame(uint64_t frame_number, int number_frames) {
    // Mark the frame as deallocated in the bitmap
    // Decrease the frames_allocated, increase frames_available
    if (!frames_allocated)
        return -1;
    int ret_val = number_frames;
    for (uint64_t i = frame_number; i < frame_number + number_frames; ++i) {
        if (!BIT_VALUE(bitmap[i / 64], i % 64))
            ret_val = -1;
        else {
            bit_off(&bitmap[i / 64], i % 64);
            frames_available++;
            frames_allocated--;
        }
    }
    return ret_val; // Return according to what's documented in the header file for this module
}
