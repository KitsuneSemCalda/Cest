# Skip e Only - Modificadores de Seleção de Testes

Os macros `skip()` e `only()` permitem execução seletiva de testes para desenvolvimento focado e debug.

## Visão Geral

| Macro | Comportamento | Caso de Uso |
|-------|--------------|-----------|
| `it()` | Executa teste normalmente | Teste padrão |
| `skip()` | Marca teste como pendente, não executa | Problemas conhecidos, trabalho em progresso |
| `only()` | Executa APENAS este teste, ignora os outros | Debug focado |

## Requisitos

Para usar skip/only, compile com:

```bash
gcc -DCEST_ENABLE_SKIP -o test examples/skip_only.c
```

Ou defina no seu source antes de incluir cest.h:

```c
#define CEST_ENABLE_SKIP
#include "cest.h"
```

---

## O Macro `skip()`

Marca testes como pendentes sem executá-los.

### Sintaxe

```c
skip("descrição do teste", {
    // Código do teste - NÃO será executado
});
```

### Casos de Uso

1. **Falhas Conhecidas** - Testes para bugs não corrigidos ainda
2. **Trabalho em Progresso** - Funcionalidades em desenvolvimento
3. **Específico de Plataforma** - Testes que precisam de infraestrutura específica
4. **Debug Temporário** - Ignora testes rapidamente para focar em outros

### Exemplo

```c
int main(int argc, char* argv[]) {
    cest_init(argc, argv);

    describe("Suite de Matemática", {
        it("soma números", {
            expect(2 + 2).toEqual(4);
        });

        skip("subtração está quebrada", {
            // Este teste não vai rodar
            expect(5 - 3).toEqual(2);
        });

        it("multiplica números", {
            expect(3 * 4).toEqual(12);
        });
    });

    return cest_result();
}
```

### Saída

```
● Suite de Matemática
  soma números
    ✓ 2 + 2 to equal 4
  subtração está quebrada
    ⊘ SKIPPED (pendente)
  multiplica números
    ✓ 3 * 4 to equal 12

Test Suites Summary:
  2 passed, 1 skipped
```

---

## O Macro `only()`

Executa APENAS o(s) teste(s) especificado(s), ignorando todos os outros.

### Sintaxe

```c
only("descrição do teste", {
    // Código do teste - APENAS este executa
});
```

### Casos de Uso

1. **Debug Isolado** - Foque em um teste enquanto desenvolve
2. **Otimização de Performance** - Teste um caminho crítico
3. **Teste de Regressão** - Verifique uma correção específica
4. **Demonstração** - Mostre uma funcionalidade específica

### Exemplo

```c
describe("Suite de Strings", {
    it("compara strings", {
        expect("hello").toEqual("hello");
    });

    only("correspondência de substring", {
        // APENAS este teste vai rodar!
        expect("hello world").toContain("world");
    });

    it("encontra prefixos", {
        expect("testing").toStartWith("test");
    });

    it("encontra sufixos", {
        expect("testing").toEndWith("ing");
    });
});
```

### Saída Com `only()`

```
● Suite de Strings
  compara strings
    ⊘ SKIPPED (only modifier)
  correspondência de substring
    ✓ "hello world" to contain "world"
  encontra prefixos
    ⊘ SKIPPED (only modifier)
  encontra sufixos
    ⊘ SKIPPED (only modifier)

Test Suites Summary:
  1 passed, 3 skipped
```

---

## Padrões de Workflow

### Padrão 1: Desenvolvimento Orientado por Debug

```c
describe("Desenvolvimento de Funcionalidade", {
    // Trabalhe neste teste primeiro
    only("nova funcionalidade funciona", {
        expect(new_feature(42)).toEqual(expected_value);
    });

    // Uma vez funcionando, remova only() e adicione mais testes
    it("trata casos especiais", {
        expect(new_feature(0)).toEqual(0);
    });

    // Marque testes quebrados como skip() durante refatoração
    skip("uso avançado até refatorado", {
        expect(new_feature_advanced(x)).toEqual(y);
    });
});
```

### Padrão 2: Testagem Incremental

```c
it("passo 1: funcionalidade básica", {
    // ✓ Passou
});

it("passo 2: adicione validação", {
    // ✓ Passou
});

skip("passo 3: estenda para novo requisito", {
    // Trabalhe nisto próximo
    // Execute com: only() quando pronto para testar
});
```

---

## Boas Práticas

1. **Remova antes de commitar** - Não deixe `skip()` ou `only()` no controle de versão
2. **Documente a intenção** - Use mensagens descritivas em skip
3. **Use git hooks** - Check pré-commit para skip/only:
   ```bash
   git grep "skip\|only" -- "*.c" && exit 1
   ```
4. **CI deve falhar** - Seu CI deve rejeitar código com `skip()/only()`:
   ```bash
   grep -r "only(" examples/ && echo "ERROR: only() found" && exit 1
   ```

---

## Verificação em CI

### Rejeite skip/only em CI

```yaml
# GitHub Actions
- name: Verificar por skip/only
  run: |
    if grep -r "skip\|only" examples/ --include="*.c"; then
      echo "ERROR: skip() ou only() encontrado no código"
      exit 1
    fi
```

---

## Ver também

- [Exemplos - Skip and Only](../examples/skip_only.c)
- [Referência de API](./api.md)
- [Princípios Básicos - Estrutura de Testes](./basics.md)
