# Types - Internal Types

## cest_type_t

Enumeration representing supported value types:

```c
typedef enum {
    CEST_TYPE_INT,      // Integers (long long)
    CEST_TYPE_DOUBLE,   // Floating point (double)
    CEST_TYPE_STR,      // Strings (const char*)
    CEST_TYPE_PTR,      // Generic pointers (const void*)
    CEST_TYPE_BOOL,     // Booleans (bool)
    CEST_TYPE_ARRAY,    // Arrays/Memory regions
#ifdef __OBJC__
    CEST_TYPE_OBJC_ID,  // Objective-C objects (id)
#endif
} cest_type_t;
```

## cest_value_t

Union structure that stores values of any type:

```c
typedef struct {
    cest_type_t type;
    union {
        long long i;          // CEST_TYPE_INT
        double d;             // CEST_TYPE_DOUBLE
        const char *s;        // CEST_TYPE_STR
        const void *p;        // CEST_TYPE_PTR
        bool b;               // CEST_TYPE_BOOL
#ifdef __OBJC__
        id obj;               // CEST_TYPE_OBJC_ID
#endif
    } as;
} cest_value_t;
```

## Value Wrappers

Functions to explicitly create values:

| Function | Description |
|:---|:---|
| `cest_int(v)` | Creates integer value |
| `cest_double(v)` | Creates double value |
| `cest_str(v)` | Creates string value |
| `cest_ptr(v)` | Creates pointer value |
| `cest_bool(v)` | Creates boolean value |
| `cest_value(v)` | Creates automatic value (C++ overloads, C macro) |

### Usage in C

```c
// _Generic macro for automatic type detection
expect(42).toBe(42);           // int -> CEST_TYPE_INT
expect(3.14).toBe(3.14);       // double -> CEST_TYPE_DOUBLE
expect("hello").toBe("hello"); // const char* -> CEST_TYPE_STR

// Explicit usage
expect(cest_int(42)).toBe(42);
expect(cest_str("hello")).toEqual("hello");
```

### Usage in C++

```c++
#include "cest.h"
#include <string>

// Template overloads
expect(42).toBe(42);                   // int
expect(3.14).toBeCloseTo(3.14, 0.01);  // double
expect("hello").toEqual("hello");      // const char*
expect(std::string("world")).toEqual("world"); // std::string
```

## cest_stats_t

Test statistics structure:

```c
typedef struct {
    int passed;   // Number of passed tests
    int failed;   // Number of failed tests
} cest_stats_t;

// Weak global variable (shared across compilation units)
extern cest_stats_t _cest_global_stats;
```

## cest_match_fn

Type for comparison functions:

```c
typedef int (*cest_match_fn)(cest_value_t actual, cest_value_t expected, int* diff_pos);

// Example match functions
int match_eq(cest_value_t a, cest_value_t b, int* diff_pos);
int match_gt(cest_value_t a, cest_value_t b, int* diff_pos);
int match_lt(cest_value_t a, cest_value_t b, int* diff_pos);
int match_contain(cest_value_t a, cest_value_t b, int* diff_pos);
```

## See also

- [Matchers](./matchers.md)
- [API Reference](./api.md)