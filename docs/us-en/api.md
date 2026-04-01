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

Starts an assertion with the actual value.

```c
expect(2 + 2).toBe(4);
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

## Value Wrappers

| Function | Description |
|:---|:---|
| `cest_int(v)` | Creates `cest_value_t` from integer |
| `cest_double(v)` | Creates `cest_value_t` from double |
| `cest_str(v)` | Creates `cest_value_t` from string |
| `cest_ptr(v)` | Creates `cest_value_t` from pointer |
| `cest_bool(v)` | Creates `cest_value_t` from boolean |
| `cest_value(v)` | Creates automatic value |

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