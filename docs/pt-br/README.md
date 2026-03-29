# Cest (Português)

O **Cest** é um framework de testes unitários minimalista para C, inspirado no Jest (JavaScript) e Gest (Go). Ele é distribuído como um único arquivo de cabeçalho (*header-only*), facilitando a integração em qualquer projeto C sem a necessidade de sistemas de build complexos ou dependências externas.

## Funcionalidades

- **Header-Only**: Basta incluir `cest.h` no seu projeto.
- **API Fluida**: Sintaxe intuitiva como `expect(a).toEqual(b)`.
- **Type-Safe**: Utiliza `_Generic` (C11) para suportar múltiplos tipos automaticamente.
- **Relatórios Coloridos**: Feedback visual imediato no terminal.
- **Zero Dependências**: Não requer bibliotecas externas (nem mesmo `-lm`).
- **Suporte Multi-arquivo**: Compile vários arquivos `.c` e o Cest unificará os resultados automaticamente.

## Múltiplos Arquivos de Teste

Para projetos maiores, você pode dividir seus testes em vários arquivos. Basta incluir `cest.h` em todos eles. Escolha um arquivo para conter a função `main()` que chama os testes dos outros arquivos ou simplesmente compile todos juntos se cada um tiver sua própria suíte.

### Exemplo de Estrutura:

**math_tests.c**:
```c
#include "cest.h"
void run_math_tests() {
    describe("Matemática", {
        it("soma corretamente", { expect(1+1).toBe(2); });
    });
}
```

**main.c**:
```c
#include "cest.h"
extern void run_math_tests();

int main() {
    run_math_tests();
    return cest_result();
}
```

**Compilação**:
```bash
gcc -o suite_testes main.c math_tests.c
./suite_testes
```

## Matchers Disponíveis

| Matcher | Descrição |
| :--- | :--- |
| `toBe(x)` / `toEqual(x)` | Verifica igualdade de valor ou identidade. |
| `toBeTruthy()` | Verifica se o valor é "verdadeiro". |
| `toBeFalsy()` | Verifica se o valor é "falso" ou nulo. |
| `toBeNull()` | Verifica se um ponteiro é `NULL`. |
| `toBeGreaterThan(x)` | Verifica se o valor é maior que `x`. |
| `toBeLessThan(x)` | Verifica se o valor é menor que `x`. |
| `toContain(substring)` | Verifica se uma string contém uma sub-string. |
| `toBeCloseTo(val, prec)` | Compara números decimais com uma precisão específica. |

## Instalação

Copie o arquivo `cest.h` para o seu diretório de includes e compile seu código C normalmente:

```bash
gcc -o meus_testes meus_testes.c
./meus_testes
```
