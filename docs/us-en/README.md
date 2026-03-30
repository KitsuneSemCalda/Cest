# Cest (English)

**Cest** is a minimalist unit testing framework for C, inspired by Jest (JavaScript) and Gest (Go). It supports C, C++, Objective-C, and Objective-C++ in a single header.

## Supported Languages

| Language | Extension | Compiler | Full Support |
|----------|-----------|----------|--------------|
| **C** | `.c` | `gcc` | Yes |
| **C++** | `.cpp` | `g++` | Yes |
| **Objective-C** | `.m` | `clang` or `gcc -x objective-c` | Yes |
| **Objective-C++** | `.mm` | `clang++` or `g++ -x objective-c++` | Yes |

## Key Features

- **Header-Only**: Just include `cest.h` in your project.
- **Multi-Language**: Native support for C, C++, Objective-C, and Objective-C++.
- **Fluent API**: Intuitive syntax like `expect(a).toEqual(b)`.
- **Type-Safe**: Uses `_Generic` (C11) or function overloading (C++) to automatically handle multiple types.
- **Colorized Output**: Immediate visual feedback in the terminal.
- **Zero Dependencies**: No external libraries needed (not even `-lm`).
- **Multi-file Support**: Compile multiple `.c` test files and Cest will unify results.

## Installation

Copy `cest.h` to your project and compile:

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
make              # Build all examples
make run          # Build and run all examples
make objc-deps    # Install ObjC dependencies (libobjc-dev)
```

## Installing ObjC Dependencies

To use Objective-C or Objective-C++, install the runtime library:

```bash
# Ubuntu/Debian
sudo apt install libobjc-dev

# Fedora/RHEL
sudo dnf install libobjc

# macOS
# Comes with Xcode/Developer Tools
```

## Available Matchers

| Matcher | Description |
| :--- | :--- |
| `toBe(x)` / `toEqual(x)` | Checks value equality or identity. |
| `toBeTruthy()` | Checks if the value is "truthy". |
| `toBeFalsy()` | Checks if the value is "falsy" or null. |
| `toBeNull()` | Checks if a pointer is `NULL`. |
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
    describe("C++ Tests", {
        it("supports std::string", {
            std::string s = "hello world";
            expect(s).toEqual("hello world");
        });

        it("works with pointers", {
            int x = 42;
            expect(&x).toBeTruthy();
        });
    });

    return cest_result();
}
```

### Objective-C

```objc
#import "cest.h"

int main() {
    describe("Objective-C Tests", {
        it("works with NSString", {
            NSString *s = @"hello";
            expect(s).toEqual(@"hello");
        });

        it("works with id type", {
            id obj = (id)0x1234;
            expect(obj).toBeTruthy();
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
    describe("ObjC++ Tests", {
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

For larger projects, you can split your tests into multiple files:

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

Compilation:
```bash
gcc -o test_suite main.c math_tests.c
./test_suite
```
