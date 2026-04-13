# Examples

Examples demonstrating Cest features across different languages and use cases.

## C - Tipos Básicos

```c
#include "cest.h"

static int add(int a, int b) { return a + b; }
static double divide(double a, double b) { return b != 0 ? a / b : 0; }

static int nums[] = {1, 2, 3};
static const char* strs[] = {"hello", "world"};

int main() {
    describe("C Basic Types", {
        it("should handle integers", {
            expect(2 + 2).toBe(4);
            expect(10 - 3).toEqual(7);
            expect(6 * 7).toEqual(42);
        });

        it("should handle floating point", {
            expect(3.14 + 0.86).toBeCloseTo(4.0, 0.001);
            expect(divide(10.0, 3.0)).toBeCloseTo(3.333, 0.001);
        });

        it("should handle booleans", {
            expect(true).toBeTruthy();
            expect(false).toBeFalsy();
            expect(1).toBeTruthy();
            expect(0).toBeFalsy();
        });

        it("should compare strings", {
            expect("hello").toEqual("hello");
            expect("world").toBe("world");
            expect("hello").toContain("ell");
        });

        it("should handle pointers", {
            int x = 42;
            expect(&x).toBeTruthy();
            expect((void*)NULL).toBeNull();
        });
    });

    describe("C Advanced", {
        it("should handle comparisons", {
            expect(10).toBeGreaterThan(5);
            expect(3).toBeLessThan(7);
        });

        it("should work with functions", {
            expect(add(2, 3)).toBe(5);
            expect(add(-1, 1)).toEqual(0);
        });

        it("should handle NULL pointers", {
            int* null_ptr = NULL;
            expect(null_ptr).toBeNull();
            expect(NULL).toBeFalsy();
        });

        it("should handle arrays", {
            expect(nums).toBeTruthy();
            expect(nums[0]).toEqual(1);
            expect(nums[1]).toEqual(2);
            expect(nums[2]).toEqual(3);
        });
    });

    return cest_result();
}
```

## C++ - STL Containers

```cpp
#include "cest.h"
#include <vector>
#include <string>

int main() {
    describe("C++ Basic Types", {
        it("should handle integers", {
            expect(2 + 2).toBe(4);
            expect(10 - 3).toEqual(7);
        });

        it("should handle floating point", {
            expect(3.14 + 0.86).toBeCloseTo(4.0, 0.001);
        });

        it("should handle std::string", {
            std::string s = "hello world";
            expect(s).toEqual("hello world");
            expect(s).toContain("world");
        });
    });

    describe("C++ STL Containers", {
        it("should work with std::vector", {
            std::vector<int> v;
            v.push_back(1);
            v.push_back(2);
            v.push_back(3);
            expect(v.at(0)).toEqual(1);
            expect(v.at(1)).toEqual(2);
            expect(v.at(2)).toEqual(3);
        });
    });

    return cest_result();
}
```

## Objective-C - id Type

```objc
#import "cest.h"

int main() {
    describe("Objective-C id Type", {
        it("should handle id type", {
            id obj1 = (id)0x1234;
            id obj2 = (id)0x1234;
            expect(obj1).toBe(obj2);
        });

        it("should handle nil", {
            id nil_obj = nil;
            expect(nil_obj).toBeNull();
            expect(nil_obj).toBeFalsy();
        });

        it("should work with id pointers", {
            id obj = (id)0x5678;
            expect(obj).toBeTruthy();
        });
    });

    return cest_result();
}
```

## Multi-file Projects

```c
// test_math.c
#include "cest.h"

int add(int a, int b) { return a + b; }

void run_math_tests() {
    describe("Math", {
        it("adds correctly", { expect(add(2, 3)).toBe(5); });
        it("subtracts correctly", { expect(add(5, -3)).toBe(2); });
    });
}
```

```c
// test_string.c
#include "cest.h"

int string_len(const char* s) {
    return (int)strlen(s);
}

void run_string_tests() {
    describe("String", {
        it("calculates length", { expect(string_len("hello")).toBe(5); });
        it("empty string", { expect(string_len("")).toBe(0); });
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

## Hooks e Setup

```c
#include "cest.h"

int contador = 0;

void setup() {
    contador = 0;
    printf("Setup chamado\n");
}

void cleanup() {
    printf("Cleanup chamado, contador = %d\n", contador);
}

int main() {
    beforeAll(setup);
    afterAll(cleanup);
    
    describe("Testes de Contador", {
        beforeEach(setup);  // Reseta contador antes de cada teste
        
        it("deve incrementar", {
            contador++;
            expect(contador).toEqual(1);
        });
        
        it("deve incrementar novamente", {
            contador += 2;
            expect(contador).toEqual(2);
        });
    });
    
    return cest_result();
}
```

## Benchmarking

```c
#include "cest.h"

int main() {
    describe("Testes de Performance", {
        bench("adição simples", {
            volatile int x = 1 + 1;
        });
        
        bench("cópia de string", {
            char buffer[100];
            strcpy(buffer, "hello world");
        });
        
        bench("operações matemáticas", {
            volatile double x = 3.14159 * 2.71828 / 1.41421;
        });
    });
    
    return cest_result();
}
```

## Novos Matchers

```c
#include "cest.h"

int main() {
    describe("Novos Matchers", {
        it("verificação de intervalos", {
            expect(5).toBeInRange(1, 10);
            expect(3.14).toBeInRange(3.0, 4.0);
        });
        
        it("prefixos/sufixos de string", {
            expect("hello world").toStartWith("hello");
            expect("hello world").toEndWith("world");
        });
    });
    
    return cest_result();
}
```

## Ver também

- [Matchers](./matchers.md)
- [Matchers Customizados](./custom_matchers.md)
- [Skip e Only](./skip_only.md)
- [Referência de API](./api.md)
- [Limitações](./limitations.md)