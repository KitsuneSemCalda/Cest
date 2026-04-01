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
| `toBeGreaterThan(x)` | Verifica se o valor é maior que `x` | int, double |
| `toBeLessThan(x)` | Verifica se o valor é menor que `x` | int, double |
| `toContain(substring)` | Verifica se uma string contém uma sub-string | string |
| `toBeCloseTo(val, prec)` | Compara números decimais com precisão específica | double |

## Exemplos

### Números

```c
expect(10).toBe(10);
expect(3.14).toBeCloseTo(3.14, 0.01);
expect(100).toBeGreaterThan(50);
expect(5).toBeLessThan(10);
```

### Strings

```c
expect("hello").toEqual("hello");
expect("hello world").toContain("world");
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

## Ver também

- [Tipos](./types.md)
- [API Reference](./api.md)