# Basics - Estrutura de Testes

## Macros de Estrutura

| Macro | Descrição |
|:---|:---|
| `describe(name, block)` | Define um conjunto de testes |
| `test(name, block)` | Define um teste individual |
| `it(name, block)` | Alias para `test` |

## Exemplo Básico

```c
#include "cest.h"

int main() {
    describe("Matemática", {
        it("soma corretamente", {
            expect(2 + 2).toBe(4);
        });

        it("compara strings", {
            expect("hello").toEqual("hello");
            expect("hello").toContain("ell");
        });
    });

    return cest_result();
}
```

## Saída Esperada

```
● Matemática
  soma corretamente
    ✓ 2 + 2 to be 4
  compara strings
    ✓ "hello" to equal "hello"
    ✓ "hello" to contain "ell"

Test Suites Summary:
  All 3 tests passed!
```

## Aninhamento

```c
describe("Calculadora", {
    describe("Soma", {
        it("inteiros positivos", { expect(1+2).toBe(3); });
        it("inteiros negativos", { expect(1+-1).toBe(0); });
    });
    
    describe("Multiplicação", {
        it("por zero", { expect(5*0).toBe(0); });
    });
});
```

## Ver também

- [Matchers](./matchers.md)
- [Exemplos](./examples.md)
- [API Reference](./api.md)