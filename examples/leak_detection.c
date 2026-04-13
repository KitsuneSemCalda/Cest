#include "../cest.h"
#include <stdlib.h>
#include <string.h>

// ============================================================================
// Memory Leak Detection Example
// ============================================================================
// Cest can detect memory leaks using various methods:
// 1. Address Sanitizer (ASan) - at compile time with -fsanitize=address
// 2. Memory Sanitizer (MSan) - at compile time with -fsanitize=memory
// 3. Valgrind integration - at runtime with valgrind --leak-check=full
// 4. Built-in leak detection (if enabled)
//
// Compile options:
//   gcc -g -fsanitize=address -o test examples/leak_detection.c
//   gcc -g -o test examples/leak_detection.c && valgrind --leak-check=full ./test

typedef struct {
    char* name;
    int age;
} Person;

Person* create_person(const char* name, int age) {
    Person* p = (Person*)malloc(sizeof(Person));
    if (p) {
        p->name = (char*)malloc(strlen(name) + 1);
        if (p->name) {
            strcpy(p->name, name);
        }
        p->age = age;
    }
    return p;
}

void free_person(Person* p) {
    if (p) {
        free(p->name);
        free(p);
    }
}

char* duplicate_string(const char* src) {
    char* dst = (char*)malloc(strlen(src) + 1);
    if (dst) {
        strcpy(dst, src);
    }
    return dst;
}

int main(int argc, char* argv[]) {
    cest_init(argc, argv);

    describe("Memory Management", {
        it("allocates and frees person correctly", {
            Person* p = create_person("Alice", 30);
            expect(p).toBeTruthy();
            expect(p->age).toEqual(30);
            expect(p->name).toEqual("Alice");
            free_person(p);  // Properly free to avoid leaks
        });

        it("handles null pointers safely", {
            free_person(NULL);  // Should not crash
            expect(1).toEqual(1);
        });

        it("duplicates strings without leaks", {
            char* original = "Hello, World!";
            char* copy = duplicate_string(original);
            expect(copy).toEqual(original);
            free(copy);  // Don't forget to free!
        });

        it("creates multiple objects safely", {
            Person* p1 = create_person("Bob", 25);
            Person* p2 = create_person("Charlie", 35);
            Person* p3 = create_person("Diana", 28);
            
            expect(p1).toBeTruthy();
            expect(p2).toBeTruthy();
            expect(p3).toBeTruthy();
            
            // Free all allocations
            free_person(p1);
            free_person(p2);
            free_person(p3);
        });
    });

    // Intentional leak for demonstration (uncomment to test leak detection)
    // This would trigger ASan or Valgrind warnings
    /*
    describe("Leak Detection Demo", {
        it("intentionally leaks memory", {
            char* leaked = (char*)malloc(100);
            strcpy(leaked, "This memory is leaked");
            expect(leaked).toBeTruthy();
            // Intentionally not freeing to demonstrate detection
        });
    });
    */

    return cest_result();
}
