#include <stdio.h>
#include <stdint.h>
#include "hwlib.h"
#include "hw.h"
#include "helpers.h"

// Part 1: Pointers and Raw Memory
// Do not use memcpy(), memmove(), strcpy(), or any other library copy routine.

void swap_ints(int* a, int* b) {
    // =================> TODO: Exchange the values that a and b point to
    int temp = *a;
    *a = *b;
    *b = temp;
}

void my_memcpy(void* dest, const void* src, size_t size) {
    // =================> TODO: Cast dest and src to uint8_t* and copy size bytes in a loop
    uint8_t* d = (uint8_t*)dest;
    const uint8_t* s = (const uint8_t*) src;

    for (int i = 0; i < size; i ++) {
        *d = *s;
        d ++;
        s ++;
    }
}

bool is_little_endian(void) {
    // =================> TODO: Store a known multi-byte value in a variable, then
    //                          look at its first byte through a uint8_t* pointer
    uint32_t x = 0x11223344;
    uint8_t* ptr = (uint8_t*)&x;
    if (*ptr == 0x44) {
        return true; // LSB stored at lowest address
    }
    return false;
}

uint32_t bytes_to_u32_le(const uint8_t* bytes) {
    // =================> TODO: Combine the 4 bytes using shifts (<<) and OR (|).
    //                          bytes[0] is the least significant byte.
    
    uint32_t result = 0;
    for (int i = 0; i < 4; i++) {
        result |= (((uint32_t)bytes[i]) << i*8);
    }
    return result;

}
