#include "frame.h"

uint64_t frames_allocated;

uint64_t frames_available;

// No need to change or initialize this. This is "empty" space that simulate your physical memory.
_Alignas(4096) char memory[MEMORY_SIZE];

// You should define a bitmap to keep track of the allocated frames here.
// Look at the handout for details on size

void frame_init() {
    // Initialize global variables
    // Initialize the bitmap
}

int64_t allocate_frame(int number_frames) {
    // Consult the bitmap and return the first available frame number, marking it as allocated
    // Increase the frames_allocated, decrease frames_available

    return -1; // Return according to what's documented in the header file for this module
}

int64_t deallocate_frame(uint64_t frame_number, int number_frames) {
    // Mark the frame as deallocated in the bitmap
    // Decrease the frames_allocated, increase frames_available

    return -1; // Return according to what's documented in the header file for this module
}
