# Cest (English)

**Minimalist C Unit Testing Framework inspired by Jest and Gest.**

Cest is a lightweight, **header-only** testing framework for C and related languages. It brings the expressive syntax of modern JavaScript and Go testing tools to C-family languages.

## Supported Languages

| Language | Extension | Compiler | Full Support |
|----------|-----------|----------|:------------:|
| **C** | `.c` | `gcc`, `clang` | Yes |
| **C++** | `.cpp` | `g++`, `clang++` | Yes |
| **Objective-C** | `.m` | `clang -x objective-c` | Yes |
| **Objective-C++** | `.mm` | `clang++ -x objective-c++` | Yes |

## Installation

Copy `cest.h` to your project:

```bash
# C
gcc -std=c11 -o tests tests.c

# C++
g++ -std=c++11 -o tests tests.cpp

# Objective-C (requires libobjc-dev)
clang -x objective-c -o tests tests.m -lobjc

# Objective-C++ (requires libobjc-dev)
clang++ -x objective-c++ -o tests tests.mm -lobjc
```

Or use the Makefile:

```bash
make              # Build all examples to build/
make run          # Build and run all examples
make clean        # Remove build directory
make objc-deps   # Check/install ObjC dependencies
```

## Available Matchers

| Matcher | Description |
|:---|:---|
| `toBe(x)` / `toEqual(x)` | Checks value equality or identity. |
| `toBeTruthy()` | Checks if the value is "truthy". |
| `toBeFalsy()` | Checks if the value is "falsy" or null. |
| `toBeNull()` | Checks if a pointer is `NULL` or `nil`. |
| `toBeGreaterThan(x)` | Checks if the value is greater than `x`. |
| `toBeLessThan(x)` | Checks if the value is less than `x`. |
| `toContain(substring)` | Checks if a string contains a sub-string. |
| `toBeCloseTo(val, prec)` | Compares doubles with specific precision. |

## Examples

### C

```c
#include "cest.h"

int main() {
    describe("Math", {
        it("adds correctly", {
            expect(2 + 2).toBe(4);
        });

        it("compares strings", {
            expect("hello").toEqual("hello");
            expect("hello").toContain("ell");
        });
    });

    return cest_result();
}
```

### C++

```cpp
#include "cest.h"
#include <string>

int main() {
    describe("Strings", {
        it("supports std::string", {
            std::string s = "hello world";
            expect(s).toEqual("hello world");
        });
    });

    return cest_result();
}
```

### Objective-C

```objc
#import "cest.h"

int main() {
    describe("Objective-C", {
        it("works with id", {
            id obj = (id)0x1234;
            expect(obj).toBeTruthy();
        });

        it("works with nil", {
            id nil_obj = nil;
            expect(nil_obj).toBeNull();
        });
    });

    return cest_result();
}
```

### Objective-C++

```objc++
#import "cest.h"
#import <string>

int main() {
    describe("ObjC++", {
        it("supports std::string", {
            std::string s = "c++ string";
            expect(s).toEqual("c++ string");
        });

        it("works with ObjC id", {
            id obj = (id)0x5678;
            expect(obj).toBeTruthy();
        });
    });

    return cest_result();
}
```

## Known Limitations

1. **Brace Initialization**: Do not use brace initializers (`int arr[] = {1, 2, 3};`) inside `it()` blocks. Declare arrays outside of blocks.

2. **Special Types**: Some types like `size_t`, `std::shared_ptr`, and `std::unique_ptr` don't have native support. Cast to basic types.

## Multi-file Projects

```c
// math_tests.c
#include "cest.h"
void run_math_tests() {
    describe("Math", {
        it("adds", { expect(1+1).toBe(2); });
    });
}
```

```c
// main.c
#include "cest.h"
extern void run_math_tests();

int main() {
    run_math_tests();
    return cest_result();
}
```

```bash
gcc -o test_suite main.c math_tests.c
./test_suite
```

## License

This project is licensed under the [BSD-3 Clause License](LICENSE).
