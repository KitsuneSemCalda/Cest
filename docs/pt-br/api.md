# API Reference

## Macros de Teste

### describe(name, block)

Define um conjunto de testes (test suite).

```c
describe("Nome da Suite", {
    // testes aqui
});
```

**Parâmetros:**
- `name` - Nome da suite de testes (string)
- `block` - Bloco de código com os testes

---

### test(name, block)
### it(name, block)

Define um teste individual. `it` é um alias para `test`.

```c
it("descrição do teste", {
    expect(valor).toBe(esperado);
});
```

---

## Macros de Asserção

### expect(x)
### expect_array(x, len)

Inicia uma asserção com o valor atual.

```c
expect(2 + 2).toBe(4);

// Para arrays
int a[] = {1, 2, 3};
expect_array(a, 3).toEqualArray(b, 3);
```

**Nota:** O argumento `x` é automaticamente convertido para `cest_value_t` usando:
- Em C++: sobrecargas de template
- Em C: macro `_Generic`

---

### Matchers

| Matcher | Uso |
|:---|:---|
| `toBe(x)` | `expect(x).toBe(y)` |
| `toEqual(x)` | `expect(x).toEqual(y)` |
| `toBeGreaterThan(x)` | `expect(x).toBeGreaterThan(y)` |
| `toBeLessThan(x)` | `expect(x).toBeLessThan(y)` |
| `toContain(x)` | `expect(x).toContain(substring)` |
| `toBeNull()` | `expect(ptr).toBeNull()` |
| `toBeTruthy()` | `expect(val).toBeTruthy()` |
| `toBeFalsy()` | `expect(val).toBeFalsy()` |
| `toBeCloseTo(val, prec)` | `expect(double_val).toBeCloseTo(3.14, 0.01)` |
| `toEqualArray(x, len)` | `expect_array(arr, n).toEqualArray(arr2, n)` |

---

## Funções

### cest_result()

Retorna o resultado dos testes e打印 um resumo.

```c
int main() {
    describe("Testes", { /* ... */ });
    return cest_result();
}
```

**Retorno:**
- `0` - Todos os testes passaram
- `1` - Algum teste falhou

---

### cest_init(argc, argv)

Inicializa o Cest com argumentos de linha de comando. Permite filtrar testes.

```c
int main(int argc, char* argv[]) {
    cest_init(argc, argv);
    describe("Testes", { /* ... */ });
    return cest_result();
}
```

**Uso:**
```bash
./test "Math"          # Roda apenas testes contendo "Math"
./test                # Roda todos os testes
```

---

### Value Wrappers

| Função | Descrição |
|:---|:---|
| `cest_int(v)` | Cria `cest_value_t` de inteiro |
| `cest_double(v)` | Cria `cest_value_t` de double |
| `cest_str(v)` | Cria `cest_value_t` de string |
| `cest_ptr(v)` | Cria `cest_value_t` de ponteiro |
| `cest_bool(v)` | Cria `cest_value_t` de booleano |
| `cest_array(v, len)` | Cria `cest_value_t` de array |
| `cest_value(v)` | Cria valor automático |

---

## Macros de Configuração

Defina estas **antes** de incluir `cest.h`:

| Macro | Descrição |
|:---|:---|
| `CEST_NO_COLORS` | Desabilita output colorido |
| `CEST_THREAD_SAFE` | Habilita thread safety (requer pthreads) |
| `CEST_NO_CLI` | Desabilita parsing de argumentos CLI |
| `CEST_NO_HOOKS` | Desabilita hooks beforeEach/afterEach |
| `CEST_ENABLE_SKIP` | Habilita modificadores skip/only |
| `CEST_ENABLE_LEAK_DETECTION` | Habilita detecção de memory leak |

### Detecção de Leak

Quando habilitado, use `cest_malloc` e `cest_free` em vez de `malloc`/`free`:

```c
#define CEST_ENABLE_LEAK_DETECTION
#include "cest.h"

int main() {
    int* p = cest_malloc(sizeof(int));
    // ...
    cest_free(p);
    return cest_result();
}
```

---

## Variáveis Globais

### _cest_global_stats

Estatísticas globais de testes (compartilhada entre TUs).

```c
extern cest_stats_t _cest_global_stats;

// Acesso manual
printf("Passed: %d, Failed: %d\n", 
    _cest_global_stats.passed, 
    _cest_global_stats.failed);
```

---

## Macros de Compatibilidade

### CEST_WEAK

Macro para símbolos fracos (suporte cross-compilation unit).

```c
#ifndef CEST_WEAK
#  if defined(_MSC_VER)
#    define CEST_WEAK static
#  else
#    define CEST_WEAK __attribute__((weak))
#  endif
#endif
```

---

## Exemplos de Uso

### Múltiplos Arquivos

```c
// test_math.c
#include "cest.h"
void run_math_tests() {
    describe("Math", {
        it("adds", { expect(1+1).toBe(2); });
    });
}
```

```c
// main.c
#include "cest.h"
extern void run_math_tests();

int main() {
    run_math_tests();
    return cest_result();
}
```

Compile:
```bash
gcc -o suite main.c test_math.c
```