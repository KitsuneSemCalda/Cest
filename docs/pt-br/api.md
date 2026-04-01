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

Inicia uma asserção com o valor atual.

```c
expect(2 + 2).toBe(4);
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

### Value Wrappers

| Função | Descrição |
|:---|:---|
| `cest_int(v)` | Cria `cest_value_t` de inteiro |
| `cest_double(v)` | Cria `cest_value_t` de double |
| `cest_str(v)` | Cria `cest_value_t` de string |
| `cest_ptr(v)` | Cria `cest_value_t` de ponteiro |
| `cest_bool(v)` | Cria `cest_value_t` de booleano |
| `cest_value(v)` | Cria valor automático |

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