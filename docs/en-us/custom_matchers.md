# Custom Matchers - Advanced Extension Guide

Custom matchers allow you to extend Cest with domain-specific assertions for your project.

## Overview

Custom matchers are powerful tools for:
- Domain-specific validation (e.g., `expect(node).isValidXML()`)
- Business logic assertions (e.g., `expect(user).hasPermission("admin")`)
- Reducing test boilerplate
- Making tests more readable and maintainable

## Built-in Matchers vs Custom Matchers

### Built-in Matchers
```c
expect(value).toBe(42);
expect(string).toContain("test");
expect(pointer).toBeNull();
```

### Custom Matchers
```c
expect(config).isValidJSON();
expect(user).hasRole("admin");
expect(date).isInPast();
```

---

## Creating Simple Custom Matchers

### Syntax: `CEST_MATCHER(name, body)`

The simplest form for matchers without arguments:

```c
CEST_MATCHER(matcher_name, {
    // Check the actual value
    if (actual.type != CEST_TYPE_STRING) {
        return 0;  // Matcher failed
    }
    
    // Perform validation
    const char* str = actual.value.s;
    // ... validation logic ...
    
    return 1;  // Matcher passed
})
```

### Example 1: String Palindrome Checker

```c
CEST_MATCHER(isPalindrome, {
    if (actual.type != CEST_TYPE_STRING) return 0;
    
    const char* str = actual.value.s;
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        if (str[i] != str[len - 1 - i]) {
            return 0;
        }
    }
    return 1;
})

// Usage:
describe("Strings", {
    it("detects palindromes", {
        expect("racecar").isPalindrome();
        expect("noon").isPalindrome();
    });
});
```

### Example 2: Even Number Checker

```c
CEST_MATCHER(isEven, {
    if (actual.type != CEST_TYPE_INT) return 0;
    return (actual.value.i % 2) == 0;
})

// Usage:
it("identifies even numbers", {
    expect(2).isEven();
    expect(4).isEven();
    expect(100).isEven();
});
```

### Example 3: Valid Pointer/Array Checker

```c
CEST_MATCHER(isValidArray, {
    if (actual.type != CEST_TYPE_POINTER) return 0;
    return actual.value.p != NULL;
})

// Usage:
it("validates arrays", {
    int arr[] = {1, 2, 3};
    expect((void*)arr).isValidArray();
});
```

---

## Creating Matchers with Arguments

### Syntax: `CEST_MATCHER_WITH_ARGS(name, arg_decl, body)`

For matchers that need additional parameters:

```c
CEST_MATCHER_WITH_ARGS(matcher_name,
    type1 param1 = expected.value.field1; type2 param2 = expected.value.field2,
    {
        // Matcher logic using actual value and parameters
        return condition;
    }
)
```

### Example 1: Range Checker

```c
CEST_MATCHER_WITH_ARGS(isInRange,
    int min = expected.value.i; int max = actual.value.i,
    {
        if (actual.type != CEST_TYPE_INT) return 0;
        int val = actual.value.i;
        return val >= min && val <= max;
    }
)

// Usage:
it("checks if value is in range", {
    expect(50).isInRange(0, 100);
    expect(25).isInRange(0, 100);
});
```

### Example 2: String Length Matcher

```c
CEST_MATCHER_WITH_ARGS(hasLength,
    int expected_len = expected.value.i,
    {
        if (actual.type != CEST_TYPE_STRING) return 0;
        return strlen(actual.value.s) == expected_len;
    }
)

// Usage:
it("validates string length", {
    expect("hello").hasLength(5);
    expect("test").hasLength(4);
});
```

---

## Type System Reference

When implementing custom matchers, use these type identifiers:

```c
enum {
    CEST_TYPE_INT,      // Integer values
    CEST_TYPE_DOUBLE,   // Floating point values
    CEST_TYPE_STRING,   // C strings (char*)
    CEST_TYPE_POINTER,  // Generic pointers (void*)
    CEST_TYPE_BOOL      // Boolean values
};
```

Access values via the `cest_value_t` union:

```c
typedef union {
    int i;              // For CEST_TYPE_INT
    double d;           // For CEST_TYPE_DOUBLE
    const char* s;      // For CEST_TYPE_STRING
    void* p;            // For CEST_TYPE_POINTER
    int b;              // For CEST_TYPE_BOOL
} cest_value_data_t;
```

---

## Real-World Examples

### Example: Email Validator

```c
#include <regex.h>

CEST_MATCHER(isValidEmail, {
    if (actual.type != CEST_TYPE_STRING) return 0;
    
    regex_t regex;
    int reti = regcomp(&regex,
        "^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$",
        REG_EXTENDED);
    
    if (reti) return 0;
    
    reti = regexec(&regex, actual.value.s, 0, NULL, 0);
    regfree(&regex);
    
    return !reti;
})

// Usage:
it("validates email addresses", {
    expect("user@example.com").isValidEmail();
    expect("test.user+tag@domain.co.uk").isValidEmail();
});
```

### Example: Integer Divisibility Checker

```c
CEST_MATCHER_WITH_ARGS(isDivisibleBy,
    int divisor = expected.value.i,
    {
        if (actual.type != CEST_TYPE_INT) return 0;
        if (divisor == 0) return 0;  // Safety check
        return (actual.value.i % divisor) == 0;
    }
)

// Usage:
it("checks divisibility", {
    expect(10).isDivisibleBy(2);
    expect(15).isDivisibleBy(3);
    expect(100).isDivisibleBy(10);
});
```

### Example: Array Length Checker

```c
CEST_MATCHER_WITH_ARGS(arrayLength,
    int expected_len = expected.value.i,
    {
        if (actual.type != CEST_TYPE_POINTER) return 0;
        // Note: For actual arrays, size needs to be tracked externally
        return actual.value.p != NULL;  // At least validate non-null
    }
)
```

---

## Best Practices

1. **Type Checking**: Always verify `actual.type` before accessing value fields
2. **Meaningful Names**: Use descriptive names like `isValidJSON` not just `valid`
3. **Clear Failure Behavior**: Return 0 for failures, 1 for success
4. **Documentation**: Comment your matchers, especially complex logic
5. **Null Checks**: Handle NULL pointers gracefully
6. **Constants**: Use magic numbers sparingly; prefer named constants

---

## Combining Matchers

Custom matchers work with negation and composition:

```c
it("combines matchers", {
    // Negation
    expect(1).not().isEven();
    expect("Hello").not().isPalindrome();
    
    // Chaining with built-ins
    expect(42).isEven();
    expect(42).toBeGreaterThan(0);
});
```

---

## Testing Your Matchers

```c
int main(int argc, char* argv[]) {
    cest_init(argc, argv);
    
    describe("Custom Matcher Tests", {
        describe("isPalindrome", {
            it("accepts palindromes", {
                expect("racecar").isPalindrome();
            });
            
            it("rejects non-palindromes", {
                expect("hello").not().isPalindrome();
            });
        });
    });
    
    return cest_result();
}
```

---

## See Also

- [API Reference](./api.md) - Complete API documentation
- [Examples - Custom Matchers](../examples/advanced/custom_matchers.c) - Full working example
- [Basics](./basics.md) - Test structure fundamentals
