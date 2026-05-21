# Installation

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

## Using Makefile

```bash
make              # Build all examples to build/
make run          # Build and run all examples
make clean        # Remove build directory
make objc-deps   # Check/install ObjC dependencies
```

## Quick Start

```c
#include "cest.h"

int main() {
    describe("My Tests", {
        it("should add", {
            expect(2 + 2).toBe(4);
        });
    });
    return cest_result();
}
```

Compile and run:

```bash
gcc -o test test.c && ./test
```