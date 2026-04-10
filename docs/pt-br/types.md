# Types - Tipos Internos

## cest_type_t

Enumeração que representa os tipos de valores suportados:

```c
typedef enum {
    CEST_TYPE_INT,      // Inteiros (long long)
    CEST_TYPE_DOUBLE,   // Números de ponto flutuante (double)
    CEST_TYPE_STR,      // Strings (const char*)
    CEST_TYPE_PTR,      // Ponteiros genéricos (const void*)
    CEST_TYPE_BOOL,     // Booleanos (bool)
    CEST_TYPE_ARRAY,    // Arrays/Regiões de memória
#ifdef __OBJC__
    CEST_TYPE_OBJC_ID,  // Objetos Objective-C (id)
#endif
} cest_type_t;
```

## cest_value_t

Estrutura union que armazena valores de qualquer tipo:

```c
typedef struct {
    cest_type_t type;
    union {
        long long i;          // CEST_TYPE_INT
        double d;             // CEST_TYPE_DOUBLE
        const char *s;        // CEST_TYPE_STR
        const void *p;         // CEST_TYPE_PTR
        bool b;               // CEST_TYPE_BOOL
#ifdef __OBJC__
        id obj;               // CEST_TYPE_OBJC_ID
#endif
    } as;
} cest_value_t;
```

## Value Wrappers

Funções para criar valores explicitamente:

| Função | Descrição |
|:---|:---|
| `cest_int(v)` | Cria um valor inteiro |
| `cest_double(v)` | Cria um valor double |
| `cest_str(v)` | Cria um valor string |
| `cest_ptr(v)` | Cria um valor ponteiro |
| `cest_bool(v)` | Cria um valor booleano |
| `cest_value(v)` | Cria valor automático (C++ overloads, C macro) |

### Uso em C

```c
// Macro _Generic para detecção automática de tipo
expect(42).toBe(42);           // int -> CEST_TYPE_INT
expect(3.14).toBe(3.14);       // double -> CEST_TYPE_DOUBLE
expect("hello").toBe("hello"); // const char* -> CEST_TYPE_STR

// Uso explícito
expect(cest_int(42)).toBe(42);
expect(cest_str("hello")).toEqual("hello");
```

### Uso em C++

```c++
#include "cest.h"
#include <string>

// Sobrecargas de template
expect(42).toBe(42);                   // int
expect(3.14).toBeCloseTo(3.14, 0.01);   // double
expect("hello").toEqual("hello");      // const char*
expect(std::string("world")).toEqual("world"); // std::string
```

## cest_stats_t

Estrutura para estatísticas de testes:

```c
typedef struct {
    int passed;   // Número de testes passou
    int failed;   // Número de testes falhou
} cest_stats_t;

// Variável global fraca (compartilhada entre unidades de compilação)
extern cest_stats_t _cest_global_stats;
```

## cest_match_fn

Tipo para funções de comparação:

```c
typedef int (*cest_match_fn)(cest_value_t actual, cest_value_t expected, int* diff_pos);

// Exemplos de funções de match
int match_eq(cest_value_t a, cest_value_t b, int* diff_pos);
int match_gt(cest_value_t a, cest_value_t b, int* diff_pos);
int match_lt(cest_value_t a, cest_value_t b, int* diff_pos);
int match_contain(cest_value_t a, cest_value_t b, int* diff_pos);
```

## Ver também

- [Matchers](./matchers.md)
- [API Reference](./api.md)