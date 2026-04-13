# Matchers Customizados - Guia de Extensão Avançada

Matchers customizados permitem estender Cest com asserções específicas de domínio para seu projeto.

## Visão Geral

Matchers customizados são poderosos para:
- Validação específica de domínio (ex: `expect(node).isValidXML()`)
- Asserções de lógica de negócio (ex: `expect(user).hasPermission("admin")`)
- Reduzir repetição em testes
- Tornar testes mais legíveis e mantíveis

## Matchers Embutidos vs Customizados

### Matchers Embutidos
```c
expect(value).toBe(42);
expect(string).toContain("test");
expect(pointer).toBeNull();
```

### Matchers Customizados
```c
expect(config).isValidJSON();
expect(user).hasRole("admin");
expect(date).isInPast();
```

---

## Criando Matchers Simples

### Sintaxe: `CEST_MATCHER(name, body)`

A forma mais simples para matchers sem argumentos:

```c
CEST_MATCHER(matcher_name, {
    // Verificar o valor atual
    if (actual.type != CEST_TYPE_STRING) {
        return 0;  // Matcher falhou
    }
    
    // Realizar validação
    const char* str = actual.value.s;
    // ... lógica de validação ...
    
    return 1;  // Matcher passou
})
```

### Exemplo 1: Verificador de Palíndromo

```c
CEST_MATCHER(isPalindrome, {
    if (actual.type != CEST_TYPE_STRING) return 0;
    
    const char* str = actual.value.s;
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        if (str[i] != str[len - 1 - i]) {
            return 0;
        }
    }
    return 1;
})

// Uso:
describe("Strings", {
    it("detecta palíndromos", {
        expect("arara").isPalindrome();
        expect("rotor").isPalindrome();
    });
});
```

### Exemplo 2: Verificador de Número Par

```c
CEST_MATCHER(isEven, {
    if (actual.type != CEST_TYPE_INT) return 0;
    return (actual.value.i % 2) == 0;
})

// Uso:
it("identifica números pares", {
    expect(2).isEven();
    expect(4).isEven();
    expect(100).isEven();
});
```

---

## Criando Matchers com Argumentos

### Sintaxe: `CEST_MATCHER_WITH_ARGS(name, arg_decl, body)`

Para matchers que precisam de parâmetros adicionais:

```c
CEST_MATCHER_WITH_ARGS(matcher_name,
    type1 param1 = expected.value.field1; type2 param2 = expected.value.field2,
    {
        // Lógica do matcher usando valor atual e parâmetros
        return condition;
    }
)
```

### Exemplo: Verificador de Intervalo

```c
CEST_MATCHER_WITH_ARGS(isInRange,
    int min = expected.value.i; int max = actual.value.i,
    {
        if (actual.type != CEST_TYPE_INT) return 0;
        int val = actual.value.i;
        return val >= min && val <= max;
    }
)

// Uso:
it("verifica se valor está no intervalo", {
    expect(50).isInRange(0, 100);
    expect(25).isInRange(0, 100);
});
```

---

## Referência do Sistema de Tipos

Ao implementar matchers customizados, use esses identificadores de tipo:

```c
enum {
    CEST_TYPE_INT,      // Valores inteiros
    CEST_TYPE_DOUBLE,   // Valores de ponto flutuante
    CEST_TYPE_STRING,   // Strings C (char*)
    CEST_TYPE_POINTER,  // Ponteiros genéricos (void*)
    CEST_TYPE_BOOL      // Valores booleanos
};
```

Acesse valores através da union `cest_value_t`:

```c
typedef union {
    int i;              // Para CEST_TYPE_INT
    double d;           // Para CEST_TYPE_DOUBLE
    const char* s;      // Para CEST_TYPE_STRING
    void* p;            // Para CEST_TYPE_POINTER
    int b;              // Para CEST_TYPE_BOOL
} cest_value_data_t;
```

---

## Boas Práticas

1. **Verificação de Tipo**: Sempre verifique `actual.type` antes de acessar campos
2. **Nomes Significativos**: Use nomes descritivos como `isValidJSON` e não só `valid`
3. **Comportamento de Falha Claro**: Retorne 0 para falha, 1 para sucesso
4. **Documentação**: Comente seus matchers, especialmente lógica complexa
5. **Verificações Nulas**: Trate ponteiros NULL com graciosidade
6. **Constantes**: Use números mágicos com parcimônia; prefira constantes nomeadas

---

## Testando Seus Matchers

```c
int main(int argc, char* argv[]) {
    cest_init(argc, argv);
    
    describe("Testes de Matchers Customizados", {
        describe("isPalindrome", {
            it("aceita palíndromos", {
                expect("arara").isPalindrome();
            });
            
            it("rejeita não-palíndromos", {
                expect("hello").not().isPalindrome();
            });
        });
    });
    
    return cest_result();
}
```

---

## Ver também

- [Referência de API](./api.md)
- [Exemplos - Matchers Customizados](../examples/advanced/custom_matchers.c)
- [Princípios Básicos](./basics.md)
