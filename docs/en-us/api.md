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

### describe_file(block)

Defines a test suite automatically named after the source file. Useful for organizing tests across multiple translation units.

```c
describe_file({
    describe("Math Tests", {
        it("adds", { expect(1+1).toBe(2); });
    });
});
```

**Output:**
```
● test_math.c
  ● Math Tests
    ✓ adds
```

This macro automatically extracts the filename from `__FILE__` and uses it as the suite name. This is useful when tests span multiple files.

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

Starts an assertion chain with the actual value. Context is automatically reset before each `expect()` call — stale data from previous assertions never leaks.

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
| `toBeInRange(min, max)` | `expect(x).toBeInRange(1, 10)` |
| `toContain(x)` | `expect(x).toContain(substring)` |
| `toStartWith(x)` | `expect(str).toStartWith(prefix)` |
| `toEndWith(x)` | `expect(str).toEndWith(suffix)` |
| `toBeNull()` | `expect(ptr).toBeNull()` |
| `toBeTruthy()` | `expect(val).toBeTruthy()` |
| `toBeFalsy()` | `expect(val).toBeFalsy()` |
| `toBeDefined()` | `expect(ptr).toBeDefined()` |
| `toBeUndefined()` | `expect(ptr).toBeUndefined()` |
| `toBeCloseTo(val, prec)` | `expect(double_val).toBeCloseTo(3.14, 0.01)` |
| `toEqualArray(x, len)` | `expect_array(arr, n).toEqualArray(arr2, n)` |
| `toMatch(regex)` | `expect(str).toMatch(std::regex("..."))` (C++ only) |

---

## Hooks

Hooks allow you to run code before and after tests.

### beforeAll(fn)
### afterAll(fn)

Run once per test suite.

```c
void setup() { /* setup code */ }
void teardown() { /* cleanup code */ }

beforeAll(setup);
afterAll(teardown);

describe("Suite", {
    it("test 1", { /* ... */ });
    it("test 2", { /* ... */ });
});
```

### beforeEach(fn)
### afterEach(fn)

Run before/after each test in a suite.

```c
void reset_state() { /* reset between tests */ }

describe("Suite", {
    beforeEach(reset_state);
    
    it("test 1", { /* ... */ });
    it("test 2", { /* ... */ });
});
```

**Note:** Hooks are disabled with `CEST_NO_HOOKS`.

When `CEST_ENABLE_SIGNAL_HANDLER` is active, crash diagnostics identify which hook caused a crash:
```
✕ CRASH: SIGSEGV (Segmentation Fault) in hook: beforeEach during test: my test
```

---

## Benchmarking

### bench(name, block)

Simple performance measurement macro.

```c
describe("Performance", {
    bench("addition", {
        volatile int x = 1 + 1;
    });
});
```

Executes the block 1000 times and reports total/average time.

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

Initialize Cest with command-line arguments. Enables test filtering and, when `CEST_ENABLE_SIGNAL_HANDLER` is defined, installs crash signal handlers.

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
| `CEST_ENABLE_FORK` | Enable test isolation via `fork()` |
| `CEST_ENABLE_COVERAGE` | Enable gcov coverage integration |
| `CEST_ENABLE_LEAK_DETECTION` | Enable memory leak detection |
| `CEST_ENABLE_SIGNAL_HANDLER` | Enable crash diagnostics (SIGSEGV, SIGABRT, etc.) |
| `CEST_PREFIX` | Use `cest_` prefix on all public macros |
| `CEST_SUPPRESS_WARNINGS` | Enable/disable compiler warning suppression (default: 1) |
| `CEST_AUTO_DESCRIBE_FILE` | Enable automatic test grouping by file |

### Warning Suppression

Cest automatically suppresses common compiler warnings to keep your test output clean. This includes warnings for:
- `-Wpedantic` - Non-standard extensions
- `-Wstrict-prototypes` - Missing function prototypes
- `-Wunused-function` - Unused functions
- `-Wunused-parameter` - Unused parameters
- `-Wimplicit-fallthrough` - Fallthrough in switches
- `-Wcast-align` - Pointer cast alignment

To disable warning suppression:
```c
#define CEST_SUPPRESS_WARNINGS 0
#include "cest.h"
```

Supported compilers: GCC, Clang, MSVC

### CI Color Output

Color output is **compile-time** only. To disable colors in CI environments:
```bash
gcc -DCEST_NO_COLORS -o my_test my_test.c
```

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

**Note:** Automatically disabled when AddressSanitizer, ThreadSanitizer, or MemorySanitizer are active to avoid conflicts.

### Signal Handler

Catches process-fatal signals (SIGSEGV, SIGABRT, SIGFPE, SIGBUS, SIGILL) and prints which test and hook were running at the time of crash. Uses `write()` (async-signal-safe) internally.

```c
#define CEST_ENABLE_SIGNAL_HANDLER
#include "cest.h"

int main(int argc, char* argv[]) {
    cest_init(argc, argv); // installs handlers
    describe("Suite", {
        it("dangerous test", {
            // if this crashes, output will be:
            // ✕ CRASH: SIGSEGV (Segmentation Fault) during test: dangerous test
        });
    });
    return cest_result();
}
```

> **Note:** `CEST_ENABLE_SIGNAL_HANDLER` is complementary to `CEST_ENABLE_FORK`. Fork provides full test isolation; the signal handler provides lightweight crash diagnostics when fork is not available or desired.

### Namespaced Macros (CEST_PREFIX)

When `CEST_PREFIX` is defined, all public macros are also available with the `cest_` prefix, avoiding name collisions with other libraries:

```c
#define CEST_PREFIX
#include "cest.h"

int main(int argc, char* argv[]) {
    cest_init(argc, argv);
    cest_describe("Suite", {
        cest_it("test", {
            cest_expect(2 + 2).toEqual(4);
        });
    });
    return cest_result();
}
```

Available prefixed macros: `cest_describe`, `cest_test`, `cest_it`, `cest_expect`, `cest_expect_array`, `cest_bench`, `cest_beforeEach`, `cest_afterEach`, `cest_beforeAll`, `cest_afterAll`.

The short-form macros (`describe`, `test`, `expect`, etc.) remain available regardless.

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

Use `describe_file()` to automatically group tests by source file:

```c
// test_math.c
#include "cest.h"
void run_math_tests() {
    describe_file({
        describe("Math", {
            it("adds", { expect(1+1).toBe(2); });
        });
    });
}
```

```c
// test_string.c
#include "cest.h"
void run_string_tests() {
    describe_file({
        describe("String", {
            it("compares", { expect("hello").toEqual("hello"); });
        });
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

Output:
```
● test_math.c
  ● Math
    ✓ adds

● test_string.c
  ● String
    ✓ compares
```