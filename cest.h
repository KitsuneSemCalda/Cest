#ifndef _CEST_H_
#define _CEST_H_

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#ifdef __OBJC__
#include <objc/objc.h>
#endif

// Portability macro for weak symbols (shared across multiple .c files)
#ifndef CEST_WEAK
#  if defined(_MSC_VER)
#    define CEST_WEAK static
#  else
#    define CEST_WEAK __attribute__((weak))
#  endif
#endif

// Internal Math Helpers (To avoid -lm dependency)
#define _CEST_ABS(x) ((x) < 0 ? -(x) : (x))

// Types and Values
typedef enum {
    CEST_TYPE_INT,
    CEST_TYPE_DOUBLE,
    CEST_TYPE_STR,
    CEST_TYPE_PTR,
    CEST_TYPE_BOOL
#ifdef __OBJC__
    , CEST_TYPE_OBJC_ID
#endif
} cest_type_t;

typedef struct {
    cest_type_t type;
    union {
        long long i;
        double d;
        const char *s;
        const void *p;
        bool b;
#ifdef __OBJC__
        id obj;
#endif
    } as;
} cest_value_t;

typedef int (*cest_match_fn)(cest_value_t actual, cest_value_t expected);

typedef struct {
    int passed;
    int failed;
} cest_stats_t;

// Shared stats across all compilation units
CEST_WEAK cest_stats_t _cest_global_stats = {0, 0};

// Value Wrappers
static inline cest_value_t cest_int(long long v) { cest_value_t cv; cv.type = CEST_TYPE_INT; cv.as.i = v; return cv; }
static inline cest_value_t cest_double(double v) { cest_value_t cv; cv.type = CEST_TYPE_DOUBLE; cv.as.d = v; return cv; }
static inline cest_value_t cest_str(const char* v) { cest_value_t cv; cv.type = CEST_TYPE_STR; cv.as.s = v ? v : "NULL"; return cv; }
static inline cest_value_t cest_ptr(const void* v) { cest_value_t cv; cv.type = CEST_TYPE_PTR; cv.as.p = v; return cv; }
static inline cest_value_t cest_bool(bool v) { cest_value_t cv; cv.type = CEST_TYPE_BOOL; cv.as.b = v; return cv; }
#ifdef __OBJC__
static inline cest_value_t cest_id(id v) { cest_value_t cv; cv.type = CEST_TYPE_OBJC_ID; cv.as.obj = v; return cv; }
#endif

#ifdef __cplusplus
#include <string>
static inline cest_value_t cest_value(bool v) { return cest_bool(v); }
static inline cest_value_t cest_value(int v) { return cest_int(v); }
static inline cest_value_t cest_value(long v) { return cest_int(v); }
static inline cest_value_t cest_value(long long v) { return cest_int(v); }
static inline cest_value_t cest_value(float v) { return cest_double(v); }
static inline cest_value_t cest_value(double v) { return cest_double(v); }
static inline cest_value_t cest_value(char* v) { return cest_str(v); }
static inline cest_value_t cest_value(const char* v) { return cest_str(v); }
static inline cest_value_t cest_value(const std::string& v) { return cest_str(v.c_str()); }
#ifdef __OBJC__
static inline cest_value_t cest_value(id v) { return cest_id(v); }
#endif
template<typename T>
static inline cest_value_t cest_value(T* v) { return cest_ptr((const void*)v); }
#else
#define cest_value(x) \
    _Generic((x), \
        _Bool: cest_bool, \
        int: cest_int, \
        long: cest_int, \
        long long: cest_int, \
        float: cest_double, \
        double: cest_double, \
        char*: cest_str, \
        const char*: cest_str, \
        _CEST_OBJC_GENERIC(x) \
        default: cest_ptr \
    )(x)

#ifdef __OBJC__
#  define _CEST_OBJC_GENERIC(x) id: cest_id,
#else
#  define _CEST_OBJC_GENERIC(x)
#endif
#endif

// Internal Context (Can be static as it's used per-expectation)
static struct {
    const char* file;
    int line;
    const char* actual_expr;
    cest_value_t actual;
} _cest_ctx;

// Core Assertion Implementation
static inline void _cest_print_value(cest_value_t v) {
    switch(v.type) {
        case CEST_TYPE_INT: printf("(%s: %lld)", "int", v.as.i); break;
        case CEST_TYPE_DOUBLE: printf("(%s: %g)", "double", v.as.d); break;
        case CEST_TYPE_STR: printf("(%s: \"%s\")", "string", v.as.s); break;
        case CEST_TYPE_PTR: printf("(%s: %p)", "pointer", v.as.p); break;
        case CEST_TYPE_BOOL: printf("(%s: %s)", "bool", v.as.b ? "true" : "false"); break;
#ifdef __OBJC__
        case CEST_TYPE_OBJC_ID: printf("(%s: %p)", "id", (void*)v.as.obj); break;
#endif
    }
}

static inline void _cest_assert_impl(cest_value_t expected, cest_match_fn match, const char* match_name, const char* expected_expr) {
    if (match(_cest_ctx.actual, expected)) {
        printf("  \033[32m✓\033[0m %s %s %s\n", _cest_ctx.actual_expr, match_name, expected_expr);
        _cest_global_stats.passed++;
    } else {
        printf("  \033[31m✕ %s failed\033[0m\n", _cest_ctx.actual_expr);
        printf("    \033[32mExpected %s: %s\033[0m\n", match_name, expected_expr);
        printf("    \033[31mReceived "); _cest_print_value(_cest_ctx.actual); printf("\033[0m\n");
        printf("    \033[90m(%s:%d)\033[0m\n", _cest_ctx.file, _cest_ctx.line);
        _cest_global_stats.failed++;
    }
}

// Matcher Logic
static inline int match_eq(cest_value_t a, cest_value_t b) {
    if (a.type != b.type) {
#ifdef __OBJC__
        if ((a.type == CEST_TYPE_PTR && b.type == CEST_TYPE_OBJC_ID) ||
            (a.type == CEST_TYPE_OBJC_ID && b.type == CEST_TYPE_PTR)) {
            const void *p = a.type == CEST_TYPE_PTR ? a.as.p : b.as.p;
            id obj = a.type == CEST_TYPE_OBJC_ID ? a.as.obj : b.as.obj;
            return p == (const void*)obj;
        }
        if ((a.type == CEST_TYPE_PTR || a.type == CEST_TYPE_OBJC_ID) && 
            (b.type == CEST_TYPE_PTR || b.type == CEST_TYPE_OBJC_ID)) {
            const void *p1 = a.type == CEST_TYPE_PTR ? a.as.p : (const void*)a.as.obj;
            const void *p2 = b.type == CEST_TYPE_PTR ? b.as.p : (const void*)b.as.obj;
            return p1 == p2;
        }
#endif
        return 0;
    }
    switch (a.type) {
        case CEST_TYPE_INT: return a.as.i == b.as.i;
        case CEST_TYPE_DOUBLE: return _CEST_ABS(a.as.d - b.as.d) < 0.000001;
        case CEST_TYPE_STR: return strcmp(a.as.s, b.as.s) == 0;
        case CEST_TYPE_PTR: return a.as.p == b.as.p;
        case CEST_TYPE_BOOL: return a.as.b == b.as.b;
#ifdef __OBJC__
        case CEST_TYPE_OBJC_ID: return a.as.obj == b.as.obj;
#endif
        default: return 0;
    }
}

static inline int match_gt(cest_value_t a, cest_value_t b) {
    if (a.type == CEST_TYPE_INT && b.type == CEST_TYPE_INT) return a.as.i > b.as.i;
    if (a.type == CEST_TYPE_DOUBLE && b.type == CEST_TYPE_DOUBLE) return a.as.d > b.as.d;
    return 0;
}

static inline int match_lt(cest_value_t a, cest_value_t b) {
    if (a.type == CEST_TYPE_INT && b.type == CEST_TYPE_INT) return a.as.i < b.as.i;
    if (a.type == CEST_TYPE_DOUBLE && b.type == CEST_TYPE_DOUBLE) return a.as.d < b.as.d;
    return 0;
}

static inline int match_contain(cest_value_t a, cest_value_t b) {
    if (a.type == CEST_TYPE_STR && b.type == CEST_TYPE_STR) return strstr(a.as.s, b.as.s) != NULL;
    return 0;
}

// Fluent API Bridge
typedef struct {
    void (*_toEqual)(cest_value_t expected, const char* expected_expr);
    void (*_toBe)(cest_value_t expected, const char* expected_expr);
    void (*_toBeGreaterThan)(cest_value_t expected, const char* expected_expr);
    void (*_toBeLessThan)(cest_value_t expected, const char* expected_expr);
    void (*_toContain)(cest_value_t expected, const char* expected_expr);
    void (*_toBeNull)(void);
    void (*_toBeTruthy)(void);
    void (*_toBeFalsy)(void);
    void (*_toBeCloseTo)(double val, double precision);
} _cest_bridge_t;

static inline void b_toEqual(cest_value_t e, const char* ee) { _cest_assert_impl(e, match_eq, "to equal", ee); }
static inline void b_toBeGreaterThan(cest_value_t e, const char* ee) { _cest_assert_impl(e, match_gt, "to be greater than", ee); }
static inline void b_toBeLessThan(cest_value_t e, const char* ee) { _cest_assert_impl(e, match_lt, "to be less than", ee); }
static inline void b_toContain(cest_value_t e, const char* ee) { _cest_assert_impl(e, match_contain, "to contain", ee); }
static inline void b_toBeNull(void) { _cest_assert_impl(cest_ptr(NULL), match_eq, "to be null", "NULL"); }
static inline void b_toBeTruthy(void) { 
    bool ok = (_cest_ctx.actual.type == CEST_TYPE_BOOL && _cest_ctx.actual.as.b) || 
              (_cest_ctx.actual.type == CEST_TYPE_INT && _cest_ctx.actual.as.i != 0) ||
              (_cest_ctx.actual.type == CEST_TYPE_STR && strlen(_cest_ctx.actual.as.s) > 0) ||
              (_cest_ctx.actual.type == CEST_TYPE_PTR && _cest_ctx.actual.as.p != NULL)
#ifdef __OBJC__
              || (_cest_ctx.actual.type == CEST_TYPE_OBJC_ID && _cest_ctx.actual.as.obj != nil)
#endif
              ;
    if (ok) { printf("  \033[32m✓\033[0m %s to be truthy\n", _cest_ctx.actual_expr); _cest_global_stats.passed++; }
    else { printf("  \033[31m✕ %s to be falsy but expected truthy\033[0m\n", _cest_ctx.actual_expr); _cest_global_stats.failed++; }
}
static inline void b_toBeFalsy(void) {
    bool ok = (_cest_ctx.actual.type == CEST_TYPE_BOOL && !_cest_ctx.actual.as.b) || 
              (_cest_ctx.actual.type == CEST_TYPE_INT && _cest_ctx.actual.as.i == 0) ||
              (_cest_ctx.actual.type == CEST_TYPE_STR && strlen(_cest_ctx.actual.as.s) == 0) ||
              (_cest_ctx.actual.type == CEST_TYPE_PTR && _cest_ctx.actual.as.p == NULL)
#ifdef __OBJC__
              || (_cest_ctx.actual.type == CEST_TYPE_OBJC_ID && _cest_ctx.actual.as.obj == nil)
#endif
              ;
    if (ok) { printf("  \033[32m✓\033[0m %s to be falsy\n", _cest_ctx.actual_expr); _cest_global_stats.passed++; }
    else { printf("  \033[31m✕ %s to be truthy but expected falsy\033[0m\n", _cest_ctx.actual_expr); _cest_global_stats.failed++; }
}
static inline void b_toBeCloseTo(double val, double precision) {
    double diff = _CEST_ABS(_cest_ctx.actual.as.d - val);
    if (diff < precision) {
        printf("  \033[32m✓\033[0m %s to be close to %g (precision %g)\n", _cest_ctx.actual_expr, val, precision);
        _cest_global_stats.passed++;
    } else {
        printf("  \033[31m✕ %s to be close to\033[0m\n", _cest_ctx.actual_expr);
        printf("    \033[32mExpected to be close to: %g (precision %g)\033[0m\n", val, precision);
        printf("    \033[31mReceived (double: %g) (diff %g)\033[0m\n", _cest_ctx.actual.as.d, diff);
        printf("    \033[90m(%s:%d)\033[0m\n", _cest_ctx.file, _cest_ctx.line);
        _cest_global_stats.failed++;
    }
}

// Shared bridge
CEST_WEAK _cest_bridge_t _cest_bridge = {
    ._toEqual = b_toEqual,
    ._toBe = b_toEqual,
    ._toBeGreaterThan = b_toBeGreaterThan,
    ._toBeLessThan = b_toBeLessThan,
    ._toContain = b_toContain,
    ._toBeNull = b_toBeNull,
    ._toBeTruthy = b_toBeTruthy,
    ._toBeFalsy = b_toBeFalsy,
    ._toBeCloseTo = b_toBeCloseTo
};

#define expect(x) (_cest_ctx.file = __FILE__, _cest_ctx.line = __LINE__, _cest_ctx.actual_expr = #x, _cest_ctx.actual = cest_value(x), _cest_bridge)

#define toEqual(x) _toEqual(cest_value(x), #x)
#define toBe(x) _toBe(cest_value(x), #x)
#define toBeGreaterThan(x) _toBeGreaterThan(cest_value(x), #x)
#define toBeLessThan(x) _toBeLessThan(cest_value(x), #x)
#define toContain(x) _toContain(cest_value(x), #x)
#define toBeNull() _toBeNull()
#define toBeTruthy() _toBeTruthy()
#define toBeFalsy() _toBeFalsy()
#define toBeCloseTo(v, p) _toBeCloseTo(v, p)

// Test Runner Macros
#define describe(name, block) do { printf("\n\033[1;34m● %s\033[0m\n", name); block } while (0)
#define test(name, block) do { printf("  %s\n", name); block } while (0)
#define it(name, block) test(name, block)

static inline int cest_result() {
    printf("\n\033[1mTest Suites Summary:\033[0m\n");
    if (_cest_global_stats.failed == 0) {
        printf("  \033[32mAll %d tests passed!\033[0m\n\n", _cest_global_stats.passed);
        return 0;
    } else {
        printf("  \033[32mPassed: %d\033[0m\n", _cest_global_stats.passed);
        printf("  \033[31mFailed: %d\033[0m\n\n", _cest_global_stats.failed);
        return 1;
    }
}

#endif
