# Matchers

Matchers são funções que verificam valores. O Cest fornece matchers fluentes através da API `expect().matcher()`.

## Matchers Disponíveis

| Matcher | Descrição | Tipos Suportados |
|:---|:---|:---|
| `toBe(x)` | Verifica igualdade de valor ou identidade | int, double, string, pointer, bool, id |
| `toEqual(x)` | Alias para `toBe` | int, double, string, pointer, bool, id |
| `toBeTruthy()` | Verifica se o valor é "verdadeiro" | int, double, string, pointer, bool, id |
| `toBeFalsy()` | Verifica se o valor é "falso" ou nulo | int, double, string, pointer, bool, id |
| `toBeNull()` | Verifica se um ponteiro é `NULL` ou `nil` | pointer, id |
| `toBeDefined()` | Verifica se um ponteiro não é `NULL`/`nil` | pointer, id |
| `toBeUndefined()` | Verifica se um ponteiro é `NULL`/`nil` | pointer, id |
| `toBeGreaterThan(x)` | Verifica se o valor é maior que `x` | int, double |
| `toBeLessThan(x)` | Verifica se o valor é menor que `x` | int, double |
| `toBeInRange(min, max)` | Verifica se o valor está dentro do intervalo | int, double |
| `toContain(substring)` | Verifica se uma string contém uma sub-string | string |
| `toStartWith(prefix)` | Verifica se uma string começa com prefixo | string |
| `toEndWith(suffix)` | Verifica se uma string termina com sufixo | string |
| `toBeCloseTo(val, prec)` | Compara números decimais com precisão específica | double |
| `toEqualArray(x, len)` | Compara duas regiões de memória | array |
| `toMatch(regex)` | Verifica string contra `std::regex` | string (apenas C++) |

## Exemplos

### Números

```c
expect(10).toBe(10);
expect(3.14).toBeCloseTo(3.14, 0.01);
expect(100).toBeGreaterThan(50);
expect(5).toBeLessThan(10);
expect(7).toBeInRange(1, 10);
```

### Strings

```c
expect("hello").toEqual("hello");
expect("hello world").toContain("world");
expect("hello world").toStartWith("hello");
expect("hello world").toEndWith("world");
expect("").toBeFalsy();
expect("non-empty").toBeTruthy();
```

### Booleanos

```c
expect(true).toBe(true);
expect(false).toBeFalsy();
expect(1).toBeTruthy();
expect(0).toBeFalsy();
```

### Ponteiros

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

### Definido / Indefinido

```c
int x = 42;
int* definido = &x;
int* indefinido = NULL;

expect(definido).toBeDefined();
expect(indefinido).toBeUndefined();
expect(indefinido).toBeNull();
```

### Regex (apenas C++)

```cpp
#include <regex>
expect("hello world").toMatch(std::regex("hello .*"));
```

## Ver também

- [Tipos](./types.md)
- [API Reference](./api.md)