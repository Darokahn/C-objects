#include <stdio.h>

#include "string.h"

int main() {
    string s;
    String(&s, "hello");
    s.print();
    s.concat("p world");
    s.print();
    s.cut(5, 1);
    s.print();

    string s2;
    String(&s2, "test");
    s2.print();
    s2.concat("ing, 1 2 3");
    s2.print();
}
