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
## Hooks

Hooks permitem executar código de setup e cleanup antes/depois dos testes.

```c
void setup() { printf("Configurando...\n"); }
void cleanup() { printf("Limpando...\n"); }

beforeAll(setup);
afterAll(cleanup);

describe("Testes de Banco", {
    beforeEach(setup);  // Executa antes de cada teste
    afterEach(cleanup); // Executa depois de cada teste
    
    it("conecta", {
        expect(conectar_bd()).toBeTruthy();
    });
    
    it("consulta", {
        expect(consultar_bd("SELECT 1")).toBeTruthy();
    });
});
```

## Benchmarking

Medição simples de performance com a macro `bench`.

```c
describe("Performance", {
    bench("concatenação de string", {
        char buffer[100];
        sprintf(buffer, "%s%s", "hello", "world");
    });
    
    bench("operações matemáticas", {
        volatile double x = 3.14159 * 2.71828;
    });
});
```

Saída:
```
● Performance
  ⚡ concatenação de string: 0.000015s total, 0.000000s avg
  ⚡ operações matemáticas: 0.000012s total, 0.000000s avg
```

## Ver também

- [Matchers](./matchers.md)
- [Exemplos](./examples.md)
- [API Reference](./api.md)