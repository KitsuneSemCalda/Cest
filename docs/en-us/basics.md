# Basics - Test Structure

## Structure Macros

| Macro | Description |
|:---|:---|
| `describe(name, block)` | Defines a test suite |
| `test(name, block)` | Defines an individual test |
| `it(name, block)` | Alias for `test` |

## Basic Example

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

## Expected Output

```
● Math
  adds correctly
    ✓ 2 + 2 to be 4
  compares strings
    ✓ "hello" to equal "hello"
    ✓ "hello" to contain "ell"

Test Suites Summary:
  All 3 tests passed!
```

## Hooks

Hooks allow setup and teardown code to run before/after tests.

```c
void setup() { printf("Setting up...\n"); }
void teardown() { printf("Cleaning up...\n"); }

beforeAll(setup);
afterAll(teardown);

describe("Database Tests", {
    beforeEach(setup);  // Runs before each test
    afterEach(teardown); // Runs after each test
    
    it("connects", {
        expect(db_connect()).toBeTruthy();
    });
    
    it("queries", {
        expect(db_query("SELECT 1")).toBeTruthy();
    });
});
```

## Benchmarking

Simple performance measurement with the `bench` macro.

```c
describe("Performance", {
    bench("string concatenation", {
        char buffer[100];
        sprintf(buffer, "%s%s", "hello", "world");
    });
    
    bench("math operations", {
        volatile double x = 3.14159 * 2.71828;
    });
});
```

Output:
```
● Performance
  ⚡ string concatenation: 0.000015s total, 0.000000s avg
  ⚡ math operations: 0.000012s total, 0.000000s avg
```

## See also

- [Matchers](./matchers.md)
- [Examples](./examples.md)
- [API Reference](./api.md)