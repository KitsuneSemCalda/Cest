# API Reference

## Test Macros

### describe(name, block)

Defines a test suite.

```c
describe("Suite Name", {
    // tests here
});
```

**Parameters:**
- `name` - Test suite name (string)
- `block` - Code block with tests

---

### test(name, block)
### it(name, block)

Defines an individual test. `it` is an alias for `test`.

```c
it("test description", {
    expect(value).toBe(expected);
});
```

---

## Assertion Macros

### expect(x)
### expect_array(x, len)

Starts an assertion with the actual value.

```c
expect(2 + 2).toBe(4);

// For arrays
int a[] = {1, 2, 3};
expect_array(a, 3).toEqualArray(b, 3);
```

**Note:** The argument `x` is automatically converted to `cest_value_t` using:
- In C++: template overloads
- In C: `_Generic` macro

---

### Matchers

| Matcher | Usage |
|:---|:---|
| `toBe(x)` | `expect(x).toBe(y)` |
| `toEqual(x)` | `expect(x).toEqual(y)` |
| `toBeGreaterThan(x)` | `expect(x).toBeGreaterThan(y)` |
| `toBeLessThan(x)` | `expect(x).toBeLessThan(y)` |
| `toContain(x)` | `expect(x).toContain(substring)` |
| `toBeNull()` | `expect(ptr).toBeNull()` |
| `toBeTruthy()` | `expect(val).toBeTruthy()` |
| `toBeFalsy()` | `expect(val).toBeFalsy()` |
| `toBeCloseTo(val, prec)` | `expect(double_val).toBeCloseTo(3.14, 0.01)` |
| `toEqualArray(x, len)` | `expect_array(arr, n).toEqualArray(arr2, n)` |

---

## Functions

### cest_result()

Returns test results and prints a summary.

```c
int main() {
    describe("Tests", { /* ... */ });
    return cest_result();
}
```

**Return:**
- `0` - All tests passed
- `1` - Some test failed

---

### cest_init(argc, argv)

Initialize Cest with command-line arguments. Enables filtering tests.

```c
int main(int argc, char* argv[]) {
    cest_init(argc, argv);
    describe("Tests", { /* ... */ });
    return cest_result();
}
```

**Usage:**
```bash
./test "Math"          # Run only tests containing "Math"
./test                # Run all tests
```

---

## Configuration Macros

Define these **before** including `cest.h`:

| Macro | Description |
|:---|:---|
| `CEST_NO_COLORS` | Disable colored output |
| `CEST_THREAD_SAFE` | Enable thread safety (requires pthreads) |
| `CEST_NO_CLI` | Disable CLI argument parsing |
| `CEST_NO_HOOKS` | Disable beforeEach/afterEach hooks |
| `CEST_ENABLE_SKIP` | Enable skip/only test modifiers |
| `CEST_ENABLE_LEAK_DETECTION` | Enable memory leak detection |

### Leak Detection

When enabled, use `cest_malloc` and `cest_free` instead of `malloc`/`free`:

```c
#define CEST_ENABLE_LEAK_DETECTION
#include "cest.h"

int main() {
    int* p = cest_malloc(sizeof(int));
    // ...
    cest_free(p);
    return cest_result();
}
```

---

## Global Variables

### _cest_global_stats

Global test statistics (shared across TUs).

```c
extern cest_stats_t _cest_global_stats;

// Manual access
printf("Passed: %d, Failed: %d\n", 
    _cest_global_stats.passed, 
    _cest_global_stats.failed);
```

---

## Compatibility Macros

### CEST_WEAK

Macro for weak symbols (cross-compilation unit support).

```c
#ifndef CEST_WEAK
#  if defined(_MSC_VER)
#    define CEST_WEAK static
#  else
#    define CEST_WEAK __attribute__((weak))
#  endif
#endif
```

---

## Usage Examples

### Multi-file Projects

```c
// test_math.c
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

Compile:
```bash
gcc -o suite main.c test_math.c
```