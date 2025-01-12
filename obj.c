#include <sys/mman.h>
#include <string.h>

char cf[] = { // minimal object code for the caller with 0x0 placeholders. Hand-written; about half the size of gcc-generated equivalent. Architecture-dependent.
    0x48, 0xba,                // movabs r10 (r10 is not used in the calling convention used by C on x86-64, and it is caller-saved)
    0, 0, 0, 0, 0, 0, 0, 0,    // 0
    0x48, 0xb8,                // movabs rax
    0, 0, 0, 0, 0, 0, 0, 0,    // 0
    0xff, 0xd2,                // call r10
    0xc3                       // ret
};
// movabs is necessary for the cpu to understand it should use the next 8 bytes as a constant. movq does not work and assumes a constant size of 4.

void *mkcaller(void* object, void* function) {
    /* 
     * clones the data in `cf` to create a private copy;
     * patches the values in it with the relevant object and method;
     * returns the new function
    */ 
    void (*call)() = mmap(NULL, sizeof(cf), PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE| MAP_ANONYMOUS, -1, 0);
    memcpy(call, cf, sizeof(cf));

    void** fnptr = (void **) (call + 2);

    void** x = (void **) (call + 12);

    *fnptr = function;
    *x = object;
    mprotect(call, sizeof(cf), PROT_READ | PROT_EXEC);
    return call;
}

