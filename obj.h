#ifndef OBJ_H
#define OBJ_H

#define SELF(type) type* self; __asm__ volatile ("mov %%r11, %0":"=r"(self)::);

void* mkcaller(void*, void*);
#endif
