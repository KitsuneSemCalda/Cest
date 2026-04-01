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

## Nesting

```c
describe("Calculator", {
    describe("Addition", {
        it("positive integers", { expect(1+2).toBe(3); });
        it("negative integers", { expect(1+-1).toBe(0); });
    });
    
    describe("Multiplication", {
        it("by zero", { expect(5*0).toBe(0); });
    });
});
```

## See also

- [Matchers](./matchers.md)
- [Examples](./examples.md)
- [API Reference](./api.md)