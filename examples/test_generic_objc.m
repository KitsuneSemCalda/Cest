#include <stdio.h>
#include <objc/objc.h>

int main() {
    id obj = NULL;
    void *ptr = NULL;
    printf("id is %s\n", _Generic((obj), id: "id", default: "other"));
    printf("ptr is %s\n", _Generic((ptr), void*: "void*", default: "other"));
    return 0;
}
