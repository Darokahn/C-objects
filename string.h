#ifndef STRING_H
#define STRING_H

typedef struct {
    char* str;
    int length;
    int capacity;

    int (*concat)(char*);
    int (*cut)(int, int);
    void (*print)();
    void (*insert)(int, char);
    void (*growTo)(int);
} string;

void String(string*, char*);
#endif
