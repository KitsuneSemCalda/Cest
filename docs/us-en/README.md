# Cest (English)

**Cest** is a minimalist unit testing framework for C, inspired by Jest (JavaScript) and Gest (Go). It's a header-only library, making it easy to integrate into any C project without complex build systems or external dependencies.

## Key Features

- **Header-Only**: Just include `cest.h` in your project.
- **Fluent API**: Intuitive syntax like `expect(a).toEqual(b)`.
- **Type-Safe**: Uses `_Generic` (C11) to automatically handle multiple types.
- **Colorized Output**: Immediate visual feedback in the terminal.
- **Zero Dependencies**: No external libraries needed (not even `-lm`).
- **Multi-file Support**: Compile multiple `.c` test files and Cest will unify results.

## Multi-file Projects

For larger projects, you can split your tests into multiple files. Just include `cest.h` in all of them. Choose one file to contain the `main()` function that calls test suites from other files.

### Example Structure:

**logic_tests.c**:
```c
#include "cest.h"
void run_logic_tests() {
    describe("Logic Suite", {
        it("should validate truthiness", { expect(1).toBeTruthy(); });
    });
}
```

**main.c**:
```c
#include "cest.h"
extern void run_logic_tests();

int main() {
    run_logic_tests();
    return cest_result();
}
```

**Compilation**:
```bash
gcc -o test_suite main.c logic_tests.c
./test_suite
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

## Installation

Copy `cest.h` to your project's include directory and compile your C code normally:

```bash
gcc -o my_tests my_tests.c
./my_tests
```
