# Matchers

Matchers are functions that verify values. Cest provides a fluent API through `expect().matcher()`.

## Available Matchers

| Matcher | Description | Supported Types |
|:---|:---|:---|
| `toBe(x)` | Checks value equality or identity | int, double, string, pointer, bool, id |
| `toEqual(x)` | Alias for `toBe` | int, double, string, pointer, bool, id |
| `toBeTruthy()` | Checks if the value is "truthy" | int, double, string, pointer, bool, id |
| `toBeFalsy()` | Checks if the value is "falsy" or null | int, double, string, pointer, bool, id |
| `toBeNull()` | Checks if a pointer is `NULL` or `nil` | pointer, id |
| `toBeGreaterThan(x)` | Checks if the value is greater than `x` | int, double |
| `toBeLessThan(x)` | Checks if the value is less than `x` | int, double |
| `toContain(substring)` | Checks if a string contains a sub-string | string |
| `toBeCloseTo(val, prec)` | Compares doubles with specific precision | double |
| `toEqualArray(x, len)` | Compares two memory regions | array |

## Examples

### Numbers

```c
expect(10).toBe(10);
expect(3.14).toBeCloseTo(3.14, 0.01);
expect(100).toBeGreaterThan(50);
expect(5).toBeLessThan(10);
```

### Strings

```c
expect("hello").toEqual("hello");
expect("hello world").toContain("world");
expect("").toBeFalsy();
expect("non-empty").toBeTruthy();
```

### Booleans

```c
expect(true).toBe(true);
expect(false).toBeFalsy();
expect(1).toBeTruthy();
expect(0).toBeFalsy();
```

### Pointers

```c
int x = 42;
expect(&x).toBeTruthy();
expect(NULL).toBeNull();
```

### Arrays

```c
int a[] = {1, 2, 3};
int b[] = {1, 2, 3};
expect_array(a, 3).toEqualArray(b, 3);
```

## See also

- [Types](./types.md)
- [API Reference](./api.md)