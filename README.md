# Cest

**Minimalist C Unit Testing Framework inspired by Jest and Gest.**

[![License](https://img.shields.io/badge/license-BSD3-blue.svg)](LICENSE)
[![Header Only](https://img.shields.io/badge/header--only-C11-orange.svg)](cest.h)
[![C++](https://img.shields.io/badge/C++-11-orange.svg)]()
[![Objective-C](https://img.shields.io/badge/ObjC-Supported-green.svg)]()

Cest is a lightweight, **header-only** testing framework for C and related languages. It brings the expressive syntax of modern JavaScript and Go testing tools to C-family languages.

---

## Supported Languages

| Language | Extension | Compiler | Full Support |
|----------|-----------|----------|:------------:|
| **C** | `.c` | `gcc`, `clang` | Yes |
| **C++** | `.cpp` | `g++`, `clang++` | Yes |
| **Objective-C** | `.m` | `clang -x objective-c` | Yes |
| **Objective-C++** | `.mm` | `clang++ -x objective-c++` | Yes |

---

## Quick Start

### C

```c
#include "cest.h"

int main() {
    describe("Math Suite", {
        it("sums numbers correctly", {
            expect(1 + 1).toEqual(2);
        });
    });

    return cest_result();
}
```

```bash
gcc -std=c11 -o test test.c
./test
```

### C++

```cpp
#include "cest.h"
#include <string>

int main() {
    describe("String Suite", {
        it("compares strings", {
            std::string s = "hello";
            expect(s).toEqual("hello");
        });
    });

    return cest_result();
}
```

```bash
g++ -std=c++11 -o test test.cpp
./test
```

### Objective-C

```objc
#import "cest.h"

int main() {
    describe("ObjC Suite", {
        it("works with id", {
            id obj = (id)0x1234;
            expect(obj).toBeTruthy();
        });
    });

    return cest_result();
}
```

```bash
clang -x objective-c -o test test.m -lobjc
./test
```

---

## Advanced Features

### Hooks

Setup and teardown code for tests:

```c
void setup() { /* initialize */ }
void teardown() { /* cleanup */ }

beforeAll(setup);
afterAll(teardown);

describe("Database Tests", {
    beforeEach(setup);
    afterEach(teardown);
    
    it("connects", {
        expect(connect_db()).toBeTruthy();
    });
});
```

### Benchmarking

Simple performance measurement:

```c
describe("Performance", {
    bench("string operations", {
        char buffer[100];
        sprintf(buffer, "test %d", 42);
    });
});
```

Output:
```
● Performance
  ⚡ string operations: 0.000015s total, 0.000000s avg
```

### CLI Filtering

Run specific tests from command line:

```bash
./test "Math"    # Run only tests containing "Math"
./test           # Run all tests
```

---

![Cest Demo](assets/basic_c.gif)
![Cest Demo](assets/basic_cpp.gif)
![Cest Demo](assets/basic_objc.gif)

---

## Features

- **Header-Only**: Just include `cest.h` in your project.
- **Multi-Language**: Native support for C, C++, Objective-C, and Objective-C++.
- **No Dependencies**: Zero external libraries (not even `-lm`).
- **Modern Syntax**: Uses `describe`, `it`, and `expect` for readable tests.
- **Colorized Output**: Instant visual feedback in your terminal.
- **Type-Safe**: Uses `_Generic` (C11) or function overloading (C++) for automatic type handling.
- **Multi-file Support**: Compile multiple `.c` test files and Cest unifies results.
- **Hooks Support**: `beforeAll`, `afterAll`, `beforeEach`, `afterEach` for setup/cleanup.
- **Built-in Benchmarking**: Simple performance measurement with `bench` macro.
- **Sanitizer Compatible**: Works seamlessly with ASan, TSan, MSan.

---

## Available Matchers

| Matcher | Description |
|:---|:---|
| `toBe(x)` / `toEqual(x)` | Checks value equality or identity. |
| `toBeTruthy()` | Checks if the value is "truthy". |
| `toBeFalsy()` | Checks if the value is "falsy" or null. |
| `toBeNull()` | Checks if a pointer is `NULL` or `nil`. |
| `toBeGreaterThan(x)` | Checks if the value is greater than `x`. |
| `toBeLessThan(x)` | Checks if the value is less than `x`. |
| `toBeInRange(min, max)` | Checks if the value is within range. |
| `toContain(substring)` | Checks if a string contains a sub-string. |
| `toStartWith(prefix)` | Checks if a string starts with prefix. |
| `toEndWith(suffix)` | Checks if a string ends with suffix. |
| `toBeCloseTo(val, prec)` | Compares doubles with specific precision. |
| `toEqualArray(arr, len)` | Compares two memory regions. |

---

## Building Examples

```bash
make              # Build all examples to build/
make run          # Build and run all examples
make clean        # Clean build directory
```

---

## Documentation

Detailed documentation is available in multiple languages:

- [Brazilian Portuguese (docs/pt-br)](docs/pt-br/overview.md)
- [English (docs/us-en)](docs/us-en/overview.md)

---

## License

This project is licensed under the [BSD-3 Clause License](LICENSE).
