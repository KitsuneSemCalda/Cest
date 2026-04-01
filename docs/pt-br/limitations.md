# Limitations - Limitações Conhecidas

## Limitações do Cest

1. **Inicialização com chaves**: Não utilize inicializadores com chaves (`int arr[] = {1, 2, 3};`) dentro de blocos `it()`. Declare arrays fora dos blocos.

2. **Tipos especiais**: Alguns tipos como `size_t`, `std::shared_ptr`, e `std::unique_ptr` não tem suporte nativo. Use casts para tipos básicos.

3. **Structs**: Para comparação de structs, use funções auxiliares com `toBeTruthy()`:

```c
typedef struct {
    int id;
    const char* nome;
} Usuario;

static bool UsuariosIguais(Usuario a, Usuario b) {
    return a.id == b.id && strcmp(a.nome, b.nome) == 0;
}

int main() {
    Usuario u1 = {1, "teste"};
    Usuario u2 = {1, "teste"};
    expect(UsuariosIguais(u1, u2)).toBeTruthy();
}
```

4. **Expressões complexas no expect()**: Evite expressões com efeitos colaterais dentro de `expect()`, pois a expressão pode ser avaliada múltiplas vezes.

5. **Variáveis locais em inicializadores**: Não use inicializadores de variáveis locais com chaves dentro de blocos `it()`.

## Soluções Alternativas

### Para tipos não suportados

```c
// size_t
expect((long long)size).toBe(42);

// std::shared_ptr
expect(ptr.get()).toBeNotNull();

// Enumerações
expect((int)meu_enum).toBe(ValorEnum);
```

### Para Structs

Use funções auxiliares ou defina comparadores:

```c
#define EXPECT_STRUCT(actual, expected, equals_fn) \
    expect(equals_fn(actual, expected)).toBeTruthy()
```

## Ver também

- [Matchers](./matchers.md)
- [Examples](./examples.md)