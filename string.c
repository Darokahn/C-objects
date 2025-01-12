#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "string.h"
#include "obj.h"

int concat(char* new) {
    SELF(string);
    int total = strlen(new) + self->length;
    if (total > self->capacity) {
        self->capacity = total * 1.5f;
        self->str = realloc(self->str, self->capacity);
    }
    strcpy(self->str + self->length, new);
    self->length = total;
}

int cut(int index, int amount) {
    SELF(string);
    if (index > self->length || index > self->length + amount) {
        return -1;
    }
    int total = self->length - amount;
    strncpy(self->str + index, self->str + index + amount, total - index);
    self->str[total] = 0;
    self->length = total;
    return total;
}

void print() {
    SELF(string);
    printf("%s\n", self->str);
}

void String(string* s, char* initial) {
    s->str = strdup(initial);
    s->length = strlen(initial);
    s->capacity = strlen(initial);

    s->concat = mkcaller(s, concat);
    s->cut = mkcaller(s, cut);
    s->print = mkcaller(s, print);
}

