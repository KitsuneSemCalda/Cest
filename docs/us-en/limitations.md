# Limitations - Known Limitations

## Cest Limitations

1. **Brace Initialization**: Do not use brace initializers (`int arr[] = {1, 2, 3};`) inside `it()` blocks. Declare arrays outside of blocks.

2. **Special Types**: Some types like `size_t`, `std::shared_ptr`, and `std::unique_ptr` don't have native support. Cast to basic types.

3. **Structs**: For struct comparison, use helper functions with `toBeTruthy()`:

```c
typedef struct {
    int id;
    const char* name;
} User;

static bool UsersEqual(User a, User b) {
    return a.id == b.id && strcmp(a.name, b.name) == 0;
}

int main() {
    User u1 = {1, "test"};
    User u2 = {1, "test"};
    expect(UsersEqual(u1, u2)).toBeTruthy();
}
```

4. **Complex expressions in expect()**: Avoid expressions with side effects inside `expect()`, as the expression may be evaluated multiple times.

5. **Local variables with brace initializers**: Do not use brace initializers for local variables inside `it()` blocks.

## Workarounds

### For unsupported types

```c
// size_t
expect((long long)size).toBe(42);

// std::shared_ptr
expect(ptr.get()).toBeNotNull();

// Enums
expect((int)my_enum).toBe(EnumValue);
```

### For Structs

Use helper functions or define comparators:

```c
#define EXPECT_STRUCT(actual, expected, equals_fn) \
    expect(equals_fn(actual, expected)).toBeTruthy()
```

## See also

- [Matchers](./matchers.md)
- [Examples](./examples.md)