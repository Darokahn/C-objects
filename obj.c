#include <sys/mman.h>
#include <string.h>
#include <stdint.h>

#define PAGE_SIZE 4096

#define CLOSURE_SIZE 24

typedef uint8_t closure[CLOSURE_SIZE];

struct {
    uint8_t spaces[PAGE_SIZE / CLOSURE_SIZE];
    uint8_t head;
    uint8_t tail;
} ALLOCATION_QUEUE = {
    .head = 0,
    .tail = 0,
    .spaces = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
        10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
        20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
        30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
        40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
        50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
        60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
        70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
        80, 81, 82, 83, 84, 85, 86, 87, 88, 89,
        90, 91, 92, 93, 94, 95, 96, 97, 98, 99,
        100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
        110, 111, 112, 113, 114, 115, 116, 117, 118, 119,
        120, 121, 122, 123, 124, 125, 126, 127, 128, 129,
        130, 131, 132, 133, 134, 135, 136, 137, 138, 139,
        140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
        150, 151, 152, 153, 154, 155, 156, 157, 158, 159,
        160, 161, 162, 163, 164, 165, 166, 167, 168, 169
    }
};

closure* ALLOCATION_PAGE = NULL;

// these functions work fine as long as you use them right. I didn't bother checking if you've allocated too many or if you double-free. You shouldn't use this code in general, so there's no reason to make it safe.
void* alloc_caller() {
    if (ALLOCATION_PAGE == NULL) {
        ALLOCATION_PAGE = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_EXEC | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    }
    void* toReturn = ALLOCATION_PAGE + ALLOCATION_QUEUE.spaces[ALLOCATION_QUEUE.head];
    ALLOCATION_QUEUE.head++;
    ALLOCATION_QUEUE.head %= PAGE_SIZE / CLOSURE_SIZE;
    return toReturn;
}

void free_caller(void* address) {
    int offset = (closure*)address - ALLOCATION_PAGE;
    ALLOCATION_QUEUE.spaces[ALLOCATION_QUEUE.tail] = offset;
    ALLOCATION_QUEUE.tail++;
    ALLOCATION_QUEUE.tail %= PAGE_SIZE / CLOSURE_SIZE;
}

char cf[] = { // minimal object code for the caller with 0x0 placeholders. Hand-written; about half the size of gcc-generated equivalent. Architecture-dependent.
    0x49, 0xba,                // movabs r10 (r10 is not used in the calling convention used by C on x86-64, and it is caller-saved)
    0, 0, 0, 0, 0, 0, 0, 0,    // 0
    0x49, 0xbb,                // movabs r11
    0, 0, 0, 0, 0, 0, 0, 0,    // 0
    0x41, 0xff, 0xe2,                // jmp r10
    0xc3                       // ret
};
// movabs is necessary for the cpu to understand it should use the next 8 bytes as a constant. movq does not work and assumes a constant size of 4.

void *mkcaller(void* object, void* function) {
    /*
     * clones the data in `cf` to create a private copy;
     * patches the values in it with the relevant object and method;
     * returns the new function
    */
    void* call = alloc_caller();
    memcpy(call, cf, sizeof(cf));

    void** fnptr = (void **) (call + 2);

    void** x = (void **) (call + 12);

    *fnptr = function;
    *x = object;
    return call;
}
