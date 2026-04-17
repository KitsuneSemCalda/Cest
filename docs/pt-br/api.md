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

Inicia uma cadeia de assertiva com o valor atual. O contexto é resetado automaticamente antes de cada chamada de `expect()` — dados antigos de assertivas anteriores nunca vazam.

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
| `toBeInRange(min, max)` | `expect(x).toBeInRange(1, 10)` |
| `toContain(x)` | `expect(x).toContain(substring)` |
| `toStartWith(x)` | `expect(str).toStartWith(prefixo)` |
| `toEndWith(x)` | `expect(str).toEndWith(sufixo)` |
| `toBeNull()` | `expect(ptr).toBeNull()` |
| `toBeTruthy()` | `expect(val).toBeTruthy()` |
| `toBeFalsy()` | `expect(val).toBeFalsy()` |
| `toBeDefined()` | `expect(ptr).toBeDefined()` |
| `toBeUndefined()` | `expect(ptr).toBeUndefined()` |
| `toBeCloseTo(val, prec)` | `expect(double_val).toBeCloseTo(3.14, 0.01)` |
| `toEqualArray(x, len)` | `expect_array(arr, n).toEqualArray(arr2, n)` |
| `toMatch(regex)` | `expect(str).toMatch(std::regex("..."))` (apenas C++) |

---

## Hooks

Hooks permitem executar código antes e depois dos testes.

### beforeAll(fn)
### afterAll(fn)

Executam uma vez por suite de testes.

```c
void setup() { /* código de setup */ }
void teardown() { /* código de cleanup */ }

beforeAll(setup);
afterAll(teardown);

describe("Suite", {
    it("teste 1", { /* ... */ });
    it("teste 2", { /* ... */ });
});
```

### beforeEach(fn)
### afterEach(fn)

Executam antes/depois de cada teste em uma suite.

```c
void reset_state() { /* reset entre testes */ }

describe("Suite", {
    beforeEach(reset_state);
    
    it("teste 1", { /* ... */ });
    it("teste 2", { /* ... */ });
});
```

**Nota:** Hooks são desabilitados com `CEST_NO_HOOKS`.

Quando `CEST_ENABLE_SIGNAL_HANDLER` está ativo, os diagnósticos de crash identificam qual hook causou a falha:
```
✕ CRASH: SIGSEGV (Segmentation Fault) in hook: beforeEach during test: meu teste
```

---

## Benchmarking

### bench(name, block)

Macro simples para medição de performance.

```c
describe("Performance", {
    bench("adição", {
        volatile int x = 1 + 1;
    });
});
```

Executa o bloco 1000 vezes e reporta tempo total/médio.

---

## Funções

### cest_result()

Retorna o resultado dos testes e exibe um resumo.

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

Inicializa o Cest com argumentos de linha de comando. Habilita o filtro de testes e, quando `CEST_ENABLE_SIGNAL_HANDLER` está definido, instala os handlers de crash.

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
| `cest_value(v)` | Cria valor automático via `_Generic` (C) ou template (C++) |

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
| `CEST_ENABLE_FORK` | Habilita isolamento de testes via `fork()` |
| `CEST_ENABLE_COVERAGE` | Habilita integração com gcov |
| `CEST_ENABLE_LEAK_DETECTION` | Habilita detecção de memory leak |
| `CEST_ENABLE_SIGNAL_HANDLER` | Habilita diagnóstico de crash (SIGSEGV, SIGABRT, etc.) |
| `CEST_PREFIX` | Usa prefixo `cest_` em todos os macros públicos |

### Output Colorido no CI

O controle de cores é **apenas em tempo de compilação**. Para desabilitar cores em ambientes CI:
```bash
gcc -DCEST_NO_COLORS -o my_test my_test.c
```

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

**Nota:** Automaticamente desabilitado quando AddressSanitizer, ThreadSanitizer ou MemorySanitizer estão ativos para evitar conflitos.

### Signal Handler

Captura sinais fatais do processo (SIGSEGV, SIGABRT, SIGFPE, SIGBUS, SIGILL) e exibe qual teste e hook estavam em execução no momento do crash. Usa `write()` internamente (async-signal-safe).

```c
#define CEST_ENABLE_SIGNAL_HANDLER
#include "cest.h"

int main(int argc, char* argv[]) {
    cest_init(argc, argv); // instala os handlers
    describe("Suite", {
        it("teste perigoso", {
            // se crashar, o output será:
            // ✕ CRASH: SIGSEGV (Segmentation Fault) during test: teste perigoso
        });
    });
    return cest_result();
}
```

> **Nota:** `CEST_ENABLE_SIGNAL_HANDLER` é complementar ao `CEST_ENABLE_FORK`. Fork oferece isolamento completo; o signal handler oferece diagnósticos leves quando fork não está disponível.

### Macros com Prefixo (CEST_PREFIX)

Quando `CEST_PREFIX` é definido, todos os macros públicos ficam disponíveis também com o prefixo `cest_`, evitando colisões de nome com outras bibliotecas:

```c
#define CEST_PREFIX
#include "cest.h"

int main(int argc, char* argv[]) {
    cest_init(argc, argv);
    cest_describe("Suite", {
        cest_it("teste", {
            cest_expect(2 + 2).toEqual(4);
        });
    });
    return cest_result();
}
```

Macros prefixados disponíveis: `cest_describe`, `cest_test`, `cest_it`, `cest_expect`, `cest_expect_array`, `cest_bench`, `cest_beforeEach`, `cest_afterEach`, `cest_beforeAll`, `cest_afterAll`.

Os macros curtos (`describe`, `test`, `expect`, etc.) continuam disponíveis independentemente.

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
        it("soma", { expect(1+1).toBe(2); });
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