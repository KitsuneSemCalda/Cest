# Memory Leak Detection

Cest provides comprehensive memory leak detection through multiple mechanisms suitable for different development workflows.

## Detection Methods

| Method | Tool | Overhead | Use Case |
|--------|------|----------|----------|
| **AddressSanitizer** | ASan | Low (~2x slower) | Development & CI |
| **Memory Sanitizer** | MSan | Medium | Uninitialized memory |
| **Thread Sanitizer** | TSan | Medium | Multi-threaded code |
| **Valgrind** | Memcheck | High (~10-50x) | Deep analysis |
| **Built-in** | Cest native | None | Reference counting |

---

## AddressSanitizer (ASan)

### Compilation

```bash
gcc -g -fsanitize=address -o test examples/leak_detection.c
g++ -g -fsanitize=address -o test examples/leak_detection.cpp
```

### Running Tests

```bash
ASAN_OPTIONS=halt_on_error=1 ./test
```

### Output Example

```
=================================================================
==12345==ERROR: LeakSanitizer: detected memory leaks

Direct leak of 100 byte(s) in 1 object(s) allocated from:
    #0 0x7f1234 in malloc (+0x1234) (test+0x1234)
    #1 0x7f5678 in create_person (/path/examples/leak_detection.c:25:20)
    #2 0x7f9012 in main (/path/examples/leak_detection.c:45:19)

SUMMARY: AddressSanitizer: 100 byte(s) leaked in 1 allocation(s).
```

### Configuration

Control ASan behavior with environment variables:

```bash
# Stop on first leak
export ASAN_OPTIONS=halt_on_error=1

# Include full stack traces
export ASAN_OPTIONS=verbosity=1

# Use suppressions file for known leaks
export ASAN_SUPPRESSIONS=asan.supp

# Full example
ASAN_OPTIONS=halt_on_error=1:verbosity=1 ./test --junit report.xml
```

---

## Memory Sanitizer (MSan)

Detects use of uninitialized memory:

### Compilation

```bash
# Requires instrumented standard library
clang -g -fsanitize=memory -fsanitize-memory-track-origins \
      -o test examples/leak_detection.c
```

### Example - Detecting Uninitialized Memory

```c
it("detects uninitialized memory", {
    int uninit;  // Uninitialized!
    expect(uninit).toEqual(0);  // MSan warning
});
```

### Output

```
WARNING: MemorySanitizer: use of uninitialized value
    #0 0x7f1234 in main (/path/examples/leak_detection.c:45:10)
```

---

## ValGrind Integration

### Compilation

```bash
# Compile without sanitizers (Valgrind doesn't work well with them)
gcc -g -o test examples/leak_detection.c
```

### Running with Valgrind

```bash
# Basic leak check
valgrind --leak-check=full ./test

# With detailed output
valgrind --leak-check=full --show-leak-kinds=all \
         --track-origins=yes ./test

# Generate suppressions for known leaks
valgrind --gen-suppressions=all ./test > suppressions.txt

# Use suppressions to ignore known leaks
valgrind --suppressions=common-leaks.supp ./test
```

### Output Example

```
==12345== HEAP SUMMARY:
==12345==     in use at exit: 100 bytes in 1 blocks
==12345==   total heap alloc+freed: 1,000 bytes in 50 blocks
==12345==   total malloc'd: 1,000 bytes
==12345==   stacks: ignored
==12345==
==12345== LEAK SUMMARY:
==12345==    definitely lost: 100 bytes in 1 blocks
==12345==    indirectly lost: 0 bytes in 0 blocks
==12345==      possibly lost: 0 bytes in 0 blocks
==12345==    still reachable: 0 bytes in 0 blocks
==12345==         suppressed: 0 bytes in 0 blocks
==12345==
```

### Leak Categories

- **Definitely lost**: Real leaks - must fix
- **Indirectly lost**: Leaked from container pointers - fix parent leak
- **Possibly lost**: May be leaks - investigate
- **Still reachable**: Not freed but pointer still exists - usually OK
- **Suppressed**: Ignored by suppressions file

---

## Best Practices

### 1. Daily Usage with ASan

```bash
#!/bin/bash
# test.sh - Run tests with memory checking

CC="gcc -g -fsanitize=address"
CFLAGS="-Wall -Wextra -std=c11"

$CC $CFLAGS -o test examples/*.c

# Run with strict settings
ASAN_OPTIONS=halt_on_error=1 ./test $@
```

### 2. Proper Memory Management Pattern

```c
typedef struct {
    char* resources;
} Resource;

Resource* create_resource() {
    Resource* r = (Resource*)malloc(sizeof(Resource));
    if (!r) return NULL;
    
    r->resources = (char*)malloc(100);
    if (!r->resources) {
        free(r);  // Cleanup on failure!
        return NULL;
    }
    return r;
}

void free_resource(Resource* r) {
    if (!r) return;
    free(r->resources);
    free(r);
}

int main(int argc, char* argv[]) {
    cest_init(argc, argv);
    
    describe("Resource Management", {
        it("allocates without leaks", {
            Resource* r = create_resource();
            expect(r).toBeTruthy();
            expect(r->resources).toBeTruthy();
            free_resource(r);  // Always cleanup!
        });
    });
    
    return cest_result();
}
```

### 3. Using Scope Guards (C11)

```c
#define CLEANUP(var, cleanup_fn) \
    __attribute__((cleanup(cleanup_fn))) typeof(var) var

void cleanup_resource(Resource** r) {
    if (r && *r) free_resource(*r);
}

it("uses automatic cleanup", {
    CLEANUP(r, cleanup_resource) Resource* res = create_resource();
    expect(res).toBeTruthy();
    // Automatically freed when scope exits!
});
```

### 4. CI Pipeline with Leak Detection

```yaml
# GitHub Actions example
- name: Run tests with memory checking
  run: |
    gcc -g -fsanitize=address -o test examples/*.c
    ASAN_OPTIONS=halt_on_error=1 ./test --junit report.xml
```

---

## Common Leak Patterns

### Pattern 1: Early Return Without Cleanup

```c
❌ LEAK: 
Resource* r = malloc(sizeof(Resource));
if (!r) return -1;  // Never freed!
process(r);

✅ FIX:
Resource* r = malloc(sizeof(Resource));
if (!r) return -1;
int ret = process(r);
free(r);
return ret;
```

### Pattern 2: Unclosed File Handles

```c
❌ LEAK:
FILE* f = fopen("data.txt", "r");
if (!f) return -1;

✅ FIX:
FILE* f = fopen("data.txt", "r");
if (!f) return -1;
// ... use file ...
fclose(f);
```

### Pattern 3: Missing Array Cleanups

```c
❌ LEAK:
char** strings = malloc(10 * sizeof(char*));
for (int i = 0; i < 10; i++) {
    strings[i] = malloc(100);
}
free(strings);  // Only freed outer array!

✅ FIX:
for (int i = 0; i < 10; i++) {
    free(strings[i]);  // Free each element first
}
free(strings);
```

---

## Suppression Files

### Creating Suppressions

For false positives or acceptable leaks:

```bash
valgrind --gen-suppressions=all --suppressions=empty.supp ./test
```

### Suppression Format

```
{
   <insert_a_suppression_name_here>
   Memcheck:Leak
   match-leak-kinds: reachable
   fun:malloc
   fun:create_person
}
```

---

## Debugging with GDB + Valgrind

```bash
valgrind --vgdb=yes --vgdb-error=0 ./test
# In another terminal:
gdb ./test
(gdb) target remote | vgdb
(gdb) break main
(gdb) continue
```

---

## Performance Considerations

| Tool | Overhead | Speed | Accuracy |
|------|----------|-------|----------|
| None | 0x | Fast | No detection |
| ASan | ~2x | Good | Excellent |
| MSan | ~3x | Good | Excellent |
| TSan | ~10x | Slow | Excellent |
| Valgrind | ~20-50x | Very slow | Excellent |

Choose based on development phase:
- **Development**: Use ASan (best balance)
- **Debug sessions**: Use Valgrind (most detailed)
- **CI**: Use ASan for speed
- **Release preparation**: Full Valgrind/ASan passes

---

## See Also

- [Examples - Leak Detection](../examples/leak_detection.c)
- [API Reference](./api.md)
- [ASan Documentation](https://github.com/google/sanitizers/wiki/AddressSanitizer)
- [Valgrind Manual](https://valgrind.org/docs/manual/)
