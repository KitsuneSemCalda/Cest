# Skip and Only Test Modifiers

The `skip()` and `only()` macros allow selective test execution for focused development and debugging.

## Overview

| Macro | Behavior | Use Case |
|-------|----------|----------|
| `it()` | Normal test execution | Standard test |
| `skip()` | Mark test as pending, don't execute | Known issues, work-in-progress |
| `only()` | Run ONLY this test, skip all others | Focused debugging |

## Requirements

To use skip/only modifiers, compile with:

```bash
gcc -DCEST_ENABLE_SKIP -o test examples/skip_only.c
```

Or define in your source before including cest.h:

```c
#define CEST_ENABLE_SKIP
#include "cest.h"
```

---

## The `skip()` Macro

Mark tests as pending without executing them.

### Syntax

```c
skip("test description", {
    // Test code - will NOT execute
});
```

### Use Cases

1. **Known Failures** - Tests for bugs not yet fixed
2. **Work in Progress** - Features under development
3. **Platform-Specific** - Tests that need specific infrastructure
4. **Temporary Debugging** - Quickly skip tests to focus on others

### Example

```c
int main(int argc, char* argv[]) {
    cest_init(argc, argv);

    describe("Math Suite", {
        it("adds numbers", {
            expect(2 + 2).toEqual(4);
        });

        skip("subtraction is broken", {
            // This test won't run
            // But the code shows intent to fix it
            expect(5 - 3).toEqual(2);
        });

        it("multiplies numbers", {
            expect(3 * 4).toEqual(12);
        });
    });

    return cest_result();
}
```

### Output

```
● Math Suite
  adds numbers
    ✓ 2 + 2 to equal 4
  subtraction is broken
    ⊘ SKIPPED (pending)
  multiplies numbers
    ✓ 3 * 4 to equal 12

Test Suites Summary:
  2 passed, 1 skipped
```

---

## The `only()` Macro

Run ONLY the specified test(s), skipping all others.

### Syntax

```c
only("test description", {
    // Test code - ONLY this runs
});
```

### Use Cases

1. **Isolated Debugging** - Focus on one test while developing
2. **Performance Optimization** - Test one critical path
3. **Regression Testing** - Verify specific fix
4. **Demonstration** - Show specific feature

### Example

```c
describe("String Suite", {
    it("compares strings", {
        expect("hello").toEqual("hello");
    });

    only("substring matching", {
        // ONLY this test will run!
        // All others (including above) will be skipped
        expect("hello world").toContain("world");
    });

    it("finds prefixes", {
        expect("testing").toStartWith("test");
    });

    it("finds suffixes", {
        expect("testing").toEndWith("ing");
    });
});
```

### Output With `only()`

```
● String Suite
  compares strings
    ⊘ SKIPPED (only modifier)
  substring matching
    ✓ "hello world" to contain "world"
  finds prefixes
    ⊘ SKIPPED (only modifier)
  finds suffixes
    ⊘ SKIPPED (only modifier)

Test Suites Summary:
  1 passed, 3 skipped
```

---

## Workflow Patterns

### Pattern 1: Debug-Driven Development

```c
describe("Feature Development", {
    // Work on this test first
    only("new feature works", {
        expect(new_feature(42)).toEqual(expected_value);
    });

    // Once working, remove only() and add more tests
    it("handles edge cases", {
        expect(new_feature(0)).toEqual(0);
    });

    // Mark broken tests as skip() during refactoring
    skip("advanced usage until refactored", {
        expect(new_feature_advanced(x)).toEqual(y);
    });
});
```

### Pattern 2: Incremental Testing

```c
it("step 1: basic functionality", {
    // ✓ Pass
});

it("step 2: add validation", {
    // ✓ Pass
});

skip("step 3: extend for new requirement", {
    // Work on this next
    // Run with: only() when ready to test
});
```

### Pattern 3: Quick Debugging

```bash
#!/bin/bash
# test.sh - Test specific functionality

# Development: run all tests
gcc -DCEST_ENABLE_SKIP -o test test.c
./test

# Debug specific test: temporarily modify test file
# Change: it("my test", { ... })
#     to: only("my test", { ... })
gcc -DCEST_ENABLE_SKIP -o test test.c
./test  # Runs only that test
```

---

## Multiple `only()` Tests

If multiple tests use `only()`, they all run:

```c
describe("Suite", {
    only("test A", { /* runs */ });
    only("test B", { /* runs */ });
    it("test C", { /* skipped */ });
});

// Output:
// test A ✓
// test B ✓
// test C ⊘ SKIPPED
```

---

## Combining skip() and only()

Skip takes precedence - if both modifiers somehow apply:

```c
// If a test is both skipped and in an only() context:
only("maybe test", {
    skip("definitely skip this", {
        // This will be SKIPPED
    });
});
```

---

## Real-World Example

### Debugging a Failing Test

**before.c** - Tests are failing:
```c
describe("Database", {
    it("connects to database", {
        expect(db_connect()).toBeTruthy();
    });

    it("queries data", {
        expect(db_query("SELECT 1")).toBeTruthy();
    });

    it("handles disconnection", {
        db_disconnect();
        expect(db_is_connected()).toBeFalsy();
    });
});
```

**during-debug.c** - Focus on one test:
```c
describe("Database", {
    skip("connects to database", {
        // Work on this next
        expect(db_connect()).toBeTruthy();
    });

    only("queries data", {
        // Debug this first
        expect(db_query("SELECT 1")).toBeTruthy();
    });

    skip("handles disconnection", {
        db_disconnect();
        expect(db_is_connected()).toBeFalsy();
    });
});
```

**after-fix.c** - Clean up modifiers:
```c
describe("Database", {
    it("connects to database", {
        expect(db_connect()).toBeTruthy();
    });

    it("queries data", {
        expect(db_query("SELECT 1")).toBeTruthy();
    });

    it("handles disconnection", {
        db_disconnect();
        expect(db_is_connected()).toBeFalsy();
    });
});
```

---

## Best Practices

1. **Remove before commit** - Don't leave `skip()` or `only()` in version control
2. **Document intent** - Use descriptive skip messages
3. **Use git hooks** - Pre-commit check for skip/only:
   ```bash
   git grep "skip\|only" -- "*.c" && exit 1
   ```
4. **CI should fail** - Your CI should reject code with `skip()/only()`:
   ```bash
   grep -r "only(" examples/ && echo "ERROR: only() found" && exit 1
   ```

---

## Git Workflow Example

```bash
# Create feature branch
git checkout -b feature/new-tests

# Add test with only() for focused development
only("new feature", { /* ... */ });

# Develop and test
make test

# Remove skip/only before committing
git diff test.c  # Verify only() is gone
git commit -m "Add tests for new feature"

# Pre-commit hook suggestion
cat > .git/hooks/pre-commit << 'EOF'
if grep -r "only(" . 2>/dev/null; then
    echo "ERROR: only() found in code"
    exit 1
fi
EOF
chmod +x .git/hooks/pre-commit
```

---

## CI Integration

### Reject skip/only in CI

```yaml
# GitHub Actions
- name: Check for skip/only
  run: |
    if grep -r "skip\|only" examples/ --include="*.c"; then
      echo "ERROR: skip() or only() found in code"
      exit 1
    fi
```

### Valgrind with skip

```bash
# Skip slow tests in Valgrind runs
gcc -DCEST_ENABLE_SKIP -o test test.c

# Mark slow tests with skip() in test file when running under Valgrind
SKIP_SLOW=1 valgrind ./test
```

---

## Performance Testing Pattern

```c
describe("Performance", {
    only("critical path optimization", {
        // Focus on one performance test
        bench("sort 10k elements", {
            qsort(data, 10000, sizeof(int), compare);
        });
    });

    skip("full benchmark suite - run nightly", {
        bench("comprehensive tests", {
            // Many benchmarks here
        });
    });
});
```

---

## See Also

- [Examples - Skip and Only](../examples/skip_only.c)
- [API Reference](./api.md)
- [Basics - Test Structure](./basics.md)
