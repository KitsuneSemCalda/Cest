# Examples

Examples demonstrating Cest features across different languages and use cases.

## C - Basic Types

```c
#include "cest.h"

static int add(int a, int b) { return a + b; }
static double divide(double a, double b) { return b != 0 ? a / b : 0; }

static int nums[] = {1, 2, 3};
static const char* strs[] = {"hello", "world"};

int main() {
    describe("C Basic Types", {
        it("should handle integers", {
            expect(2 + 2).toBe(4);
            expect(10 - 3).toEqual(7);
            expect(6 * 7).toEqual(42);
        });

        it("should handle floating point", {
            expect(3.14 + 0.86).toBeCloseTo(4.0, 0.001);
            expect(divide(10.0, 3.0)).toBeCloseTo(3.333, 0.001);
        });

        it("should handle booleans", {
            expect(true).toBeTruthy();
            expect(false).toBeFalsy();
            expect(1).toBeTruthy();
            expect(0).toBeFalsy();
        });

        it("should compare strings", {
            expect("hello").toEqual("hello");
            expect("world").toBe("world");
            expect("hello").toContain("ell");
        });

        it("should handle pointers", {
            int x = 42;
            expect(&x).toBeTruthy();
            expect((void*)NULL).toBeNull();
        });
    });

    describe("C Advanced", {
        it("should handle comparisons", {
            expect(10).toBeGreaterThan(5);
            expect(3).toBeLessThan(7);
        });

        it("should work with functions", {
            expect(add(2, 3)).toBe(5);
            expect(add(-1, 1)).toEqual(0);
        });

        it("should handle NULL pointers", {
            int* null_ptr = NULL;
            expect(null_ptr).toBeNull();
            expect(NULL).toBeFalsy();
        });

        it("should handle arrays", {
            expect(nums).toBeTruthy();
            expect(nums[0]).toEqual(1);
            expect(nums[1]).toEqual(2);
            expect(nums[2]).toEqual(3);
        });
    });

    return cest_result();
}
```

## C++ - STL Containers

```cpp
#include "cest.h"
#include <vector>
#include <string>

int main() {
    describe("C++ Basic Types", {
        it("should handle integers", {
            expect(2 + 2).toBe(4);
            expect(10 - 3).toEqual(7);
        });

        it("should handle floating point", {
            expect(3.14 + 0.86).toBeCloseTo(4.0, 0.001);
        });

        it("should handle std::string", {
            std::string s = "hello world";
            expect(s).toEqual("hello world");
            expect(s).toContain("world");
        });
    });

    describe("C++ STL Containers", {
        it("should work with std::vector", {
            std::vector<int> v;
            v.push_back(1);
            v.push_back(2);
            v.push_back(3);
            expect(v.at(0)).toEqual(1);
            expect(v.at(1)).toEqual(2);
            expect(v.at(2)).toEqual(3);
        });
    });

    return cest_result();
}
```

## Objective-C - id Type

```objc
#import "cest.h"

int main() {
    describe("Objective-C id Type", {
        it("should handle id type", {
            id obj1 = (id)0x1234;
            id obj2 = (id)0x1234;
            expect(obj1).toBe(obj2);
        });

        it("should handle nil", {
            id nil_obj = nil;
            expect(nil_obj).toBeNull();
            expect(nil_obj).toBeFalsy();
        });

        it("should work with id pointers", {
            id obj = (id)0x5678;
            expect(obj).toBeTruthy();
        });
    });

    return cest_result();
}
```

## Multi-file Projects

```c
// test_math.c
#include "cest.h"

int add(int a, int b) { return a + b; }

void run_math_tests() {
    describe("Math", {
        it("adds correctly", { expect(add(2, 3)).toBe(5); });
        it("subtracts correctly", { expect(add(5, -3)).toBe(2); });
    });
}
```

```c
// test_string.c
#include "cest.h"

int string_len(const char* s) {
    return (int)strlen(s);
}

void run_string_tests() {
    describe("String", {
        it("calculates length", { expect(string_len("hello")).toBe(5); });
        it("empty string", { expect(string_len("")).toBe(0); });
    });
}
```

```c
// main.c
#include "cest.h"

extern void run_math_tests();
extern void run_string_tests();

int main() {
    run_math_tests();
    run_string_tests();
    return cest_result();
}
```

Compile:
```bash
gcc -o suite main.c test_math.c test_string.c
```

## Hooks and Setup

```c
#include "cest.h"

int counter = 0;

void setup() {
    counter = 0;
    printf("Setup called\n");
}

void teardown() {
    printf("Teardown called, counter = %d\n", counter);
}

int main() {
    beforeAll(setup);
    afterAll(teardown);
    
    describe("Counter Tests", {
        beforeEach(setup);  // Reset counter before each test
        
        it("should increment", {
            counter++;
            expect(counter).toEqual(1);
        });
        
        it("should increment again", {
            counter += 2;
            expect(counter).toEqual(2);
        });
    });
    
    return cest_result();
}
```

## Benchmarking

```c
#include "cest.h"

int main() {
    describe("Performance Tests", {
        bench("simple addition", {
            volatile int x = 1 + 1;
        });
        
        bench("string copy", {
            char buffer[100];
            strcpy(buffer, "hello world");
        });
        
        bench("math operations", {
            volatile double x = 3.14159 * 2.71828 / 1.41421;
        });
    });
    
    return cest_result();
}
```

## New Matchers

```c
#include "cest.h"

int main() {
    describe("New Matchers", {
        it("range checks", {
            expect(5).toBeInRange(1, 10);
            expect(3.14).toBeInRange(3.0, 4.0);
        });
        
        it("string prefixes/suffixes", {
            expect("hello world").toStartWith("hello");
            expect("hello world").toEndWith("world");
        });
    });
    
    return cest_result();
}
```

## Custom Matchers

Create domain-specific matchers extending Cest with custom assertions:

```c
#include "cest.h"

// Simple custom matcher - palindrome checker
CEST_MATCHER(isPalindrome, {
    if (actual.type != CEST_TYPE_STRING) return 0;
    
    const char* str = actual.value.s;
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        if (str[i] != str[len - 1 - i]) {
            return 0;
        }
    }
    return 1;
})

// Matcher with arguments - range check
CEST_MATCHER_WITH_ARGS(isInRange,
    int min = expected.value.i; int max = actual.value.i,
    {
        if (actual.type != CEST_TYPE_INT) return 0;
        int val = actual.value.i;
        return val >= min && val <= max;
    }
)

int main(int argc, char* argv[]) {
    cest_init(argc, argv);

    describe("Custom Matchers", {
        it("detects palindromes", {
            expect("racecar").isPalindrome();
            expect("noon").isPalindrome();
        });

        it("checks ranges", {
            expect(50).isInRange(0, 100);
            expect(25).isInRange(0, 100);
        });
    });

    return cest_result();
}
```

**Compile:**
```bash
gcc -o test examples/advanced/custom_matchers.c
./test
```

For detailed guide, see [Custom Matchers Documentation](./custom_matchers.md)

---

## Skip and Only Test Modifiers

Control test execution for focused development:

```c
#include "cest.h"

int main(int argc, char* argv[]) {
    cest_init(argc, argv);

    describe("Selective Testing", {
        it("normal test runs", {
            expect(2 + 2).toEqual(4);
        });

        skip("known issue - to be fixed", {
            // This test won't execute
            expect(buggy_function()).toEqual(expected);
        });

        it("another test runs", {
            expect(3 * 4).toEqual(12);
        });
    });

    return cest_result();
}
```

**Compile with skip/only support:**
```bash
gcc -DCEST_ENABLE_SKIP -o test examples/skip_only.c
./test
```

**Using `only()` for focused debugging:**
```c
only("focus on this test", {
    // Runs ONLY this test, skipping all others
    expect(critical_function()).toEqual(42);
});
```

See [Skip and Only Documentation](./skip_only.md) for workflow patterns.

---

## Memory Leak Detection

Use ASan or Valgrind to detect memory leaks:

```c
#include "cest.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
    char* name;
    int value;
} Item;

Item* create_item(const char* name, int value) {
    Item* item = (Item*)malloc(sizeof(Item));
    if (!item) return NULL;
    
    item->name = (char*)malloc(strlen(name) + 1);
    if (!item->name) {
        free(item);
        return NULL;
    }
    
    strcpy(item->name, name);
    item->value = value;
    return item;
}

void free_item(Item* item) {
    if (!item) return;
    free(item->name);
    free(item);
}

int main(int argc, char* argv[]) {
    cest_init(argc, argv);

    describe("Memory Safety", {
        it("allocates without leaks", {
            Item* item = create_item("test", 42);
            expect(item).toBeTruthy();
            expect(item->value).toEqual(42);
            free_item(item);  // Clean up!
        });
    });

    return cest_result();
}
```

**Compile with AddressSanitizer:**
```bash
gcc -g -fsanitize=address -o test examples/leak_detection.c
./test
```

**Or with Valgrind:**
```bash
gcc -g -o test examples/leak_detection.c
valgrind --leak-check=full ./test
```

See [Leak Detection Guide](./leak_detection.md) for detailed workflows.

---

## CI/CD Integration

Output test results in JUnit XML or JSON format for CI pipelines:

```c
#include "cest.h"

int main(int argc, char* argv[]) {
    cest_init(argc, argv);

    describe("Calculator", {
        it("adds numbers", {
            expect(2 + 3).toEqual(5);
        });

        it("multiplies numbers", {
            expect(4 * 5).toEqual(20);
        });

        it("handles strings", {
            expect("hello").toContain("ello");
        });
    });

    return cest_result();
}
```

**Generate JUnit XML:**
```bash
gcc -o test examples/ci_integration.c
./test --junit report.xml
```

**Generate JSON:**
```bash
./test --json report.json
```

**GitHub Actions integration:**
```yaml
- name: Run tests
  run: gcc -o test examples/ci_integration.c && ./test --junit report.xml

- name: Publish results
  uses: dorny/test-reporter@v1
  with:
    path: report.xml
    reporter: java-junit
```

See [CI/CD Integration Guide](./ci_integration.md) for complete setup examples including Jenkins, GitLab CI, and more.

---

## See also

- [Matchers](./matchers.md)
- [Custom Matchers](./custom_matchers.md)
- [Skip and Only](./skip_only.md)
- [Leak Detection](./leak_detection.md)
- [CI/CD Integration](./ci_integration.md)
- [API Reference](./api.md)
- [Limitations](./limitations.md)