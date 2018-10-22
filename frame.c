/**
 * Functions for handling frame allocation.
 *
 * @author Ryan Strauss
 * @author Niall Williams
 */

#include "frame.h"

#define NUM_FRAMES 1024
#define BITMAP_LENGTH 16
#define BIT_VALUE(ll, i) ((ll << (63 - i)) >> 63)

uint64_t frames_allocated;

uint64_t frames_available;

uint64_t bitmap[BITMAP_LENGTH];

// No need to change or initialize this. This is "empty" space that simulate your physical memory.
_Alignas(4096) char memory[MEMORY_SIZE];

// You should define a bitmap to keep track of the allocated frames here.
// Look at the handout for details on size

void frame_init() {
    // Initialize global variables
    // Initialize the bitmap
    frames_allocated = 0;
    frames_available = NUM_FRAMES;

    for (int i = 0; i < NUM_FRAMES; ++i) {
        bitmap[i] = 0;
    }
}

int64_t allocate_frame(int number_frames) {
    // Consult the bitmap and return the first available frame number, marking it as allocated
    // Increase the frames_allocated, decrease frames_available
    if (!frames_available)
        return -1;
    for (int i = 0; i < BITMAP_LENGTH; ++i) {
        for (int j = 0; j < 64; ++j) {
            if (!BIT_VALUE(bitmap[i], j)) {
                bitmap[i] |= 1ULL << j;
                frames_allocated++;
                frames_available--;
                return i * 64 + j;
            }
        }
    }
    return -1; // Return according to what's documented in the header file for this module
}

int64_t deallocate_frame(uint64_t frame_number, int number_frames) {
    // Mark the frame as deallocated in the bitmap
    // Decrease the frames_allocated, increase frames_available
    if (!frames_allocated)
        return -1;
    bitmap[frame_number / BITMAP_LENGTH] &= ~(1UL << frame_number & BITMAP_LENGTH);
    return -1; // Return according to what's documented in the header file for this module
}

/*
 * QUESTIONS:
 *
 * 1. Are we allocating/deallocating multiple frames, or just one frame?
 * 2. What are we returning in deallocate_frame? Do we need to check if the one we try to deallocate is not
 *    already allocated?
 * 3. Are we doing things right?
 * 4. What are FRAME_NUMBER and FRAME_ADDRESS for?
 * 5. _Alignas?
 */