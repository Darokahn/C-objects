#ifndef STRING_H
#define STRING_H

typedef struct {
    char* str;
    int length;
    int capacity;

    int (*concat)(char*);
    int (*cut)(int, int);
    void (*print)();
} string;

void String(string*, char*);
#endif
