# Cest (Portugues)

**Cest** e um framework de testes unitarios minimalista para C, inspirado no Jest (JavaScript) e Gest (Go). Ele suporta C, C++, Objective-C e Objective-C++ em um unico header.

## Linguagens Suportadas

| Linguagem | Extensao | Compilador | Suporte Completo |
|-----------|----------|------------|------------------|
| **C** | `.c` | `gcc` | Sim |
| **C++** | `.cpp` | `g++` | Sim |
| **Objective-C** | `.m` | `clang` ou `gcc -x objective-c` | Sim |
| **Objective-C++** | `.mm` | `clang++` ou `g++ -x objective-c++` | Sim |

## Funcionalidades

- **Header-Only**: Basta incluir `cest.h` no seu projeto.
- **Multi-Linguagem**: Suporte nativo para C, C++, Objective-C e Objective-C++.
- **API Fluida**: Sintaxe intuitiva como `expect(a).toEqual(b)`.
- **Type-Safe**: Utiliza `_Generic` (C11) ou sobrecarga de funcoes (C++) para automaticamente lidar com multiplos tipos.
- **Relatorios Coloridos**: Feedback visual imediato no terminal.
- **Zero Dependencias**: Nao requer bibliotecas externas (nem mesmo `-lm`).
- **Suporte Multi-arquivo**: Compile varios arquivos `.c` e o Cest unificara os resultados automaticamente.

## Instalacao

Copie o arquivo `cest.h` para o seu projeto:

```bash
# C
gcc -std=c11 -o testes testes.c

# C++
g++ -std=c++11 -o testes testes.cpp

# Objective-C (requer libobjc-dev)
clang -x objective-c -o testes testes.m -lobjc

# Objective-C++ (requer libobjc-dev)
clang++ -x objective-c++ -o testes testes.mm -lobjc
```

Ou use o Makefile:

```bash
make              # Compila todos os exemplos
make run          # Compila e executa todos os exemplos
make objc-deps    # Instala dependencias ObjC (libobjc-dev)
```

## Instalando Dependencias ObjC

Para usar Objective-C ou Objective-C++, instale a biblioteca runtime:

```bash
# Ubuntu/Debian
sudo apt install libobjc-dev

# Fedora/RHEL
sudo dnf install libobjc

# macOS
# ja vem com Xcode/Developer Tools
```

## Matchers Disponiveis

| Matcher | Descricao |
| :--- | :--- |
| `toBe(x)` / `toEqual(x)` | Verifica igualdade de valor ou identidade. |
| `toBeTruthy()` | Verifica se o valor e "verdadeiro". |
| `toBeFalsy()` | Verifica se o valor e "falso" ou nulo. |
| `toBeNull()` | Verifica se um ponteiro e `NULL`. |
| `toBeGreaterThan(x)` | Verifica se o valor e maior que `x`. |
| `toBeLessThan(x)` | Verifica se o valor e menor que `x`. |
| `toContain(substring)` | Verifica se uma string contem uma sub-string. |
| `toBeCloseTo(val, prec)` | Compara numeros decimais com precisao especifica. |

## Exemplos

### C

```c
#include "cest.h"

int main() {
    describe("Matematica", {
        it("soma corretamente", {
            expect(2 + 2).toBe(4);
        });

        it("compara strings", {
            expect("hello").toEqual("hello");
            expect("hello").toContain("ell");
        });
    });

    return cest_result();
}
```

### C++

```cpp
#include "cest.h"
#include <string>

int main() {
    describe("C++ Testes", {
        it("suporta std::string", {
            std::string s = "hello world";
            expect(s).toEqual("hello world");
        });

        it("trabalha com ponteiros", {
            int x = 42;
            expect(&x).toBeTruthy();
        });
    });

    return cest_result();
}
```

### Objective-C

```objc
#import "cest.h"

int main() {
    describe("Objective-C Testes", {
        it("trabalha com NSString", {
            NSString *s = @"hello";
            expect(s).toEqual(@"hello");
        });

        it("trabalha com id", {
            id obj = (id)0x1234;
            expect(obj).toBeTruthy();
        });
    });

    return cest_result();
}
```

### Objective-C++

```objc++
#import "cest.h"
#import <string>

int main() {
    describe("ObjC++ Testes", {
        it("suporta std::string", {
            std::string s = "c++ string";
            expect(s).toEqual("c++ string");
        });

        it("trabalha com id ObjC", {
            id obj = (id)0x5678;
            expect(obj).toBeTruthy();
        });
    });

    return cest_result();
}
```

## Limitacoes Conhecidas

1. **Brace Initialization**: Nao utilize inicializadores com chaves (`int arr[] = {1, 2, 3};`) dentro de blocos `it()`. Declare arrays fora dos blocos.

2. **Tipos Especiais**: Alguns tipos como `size_t`, `std::shared_ptr`, e `std::unique_ptr` nao tem suporte nativo. Use casts para tipos basicos.

## Multiplos Arquivos de Teste

Para projetos maiores, voce pode dividir seus testes em varios arquivos:

```c
// math_tests.c
#include "cest.h"
void run_math_tests() {
    describe("Matematica", {
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

Compilacao:
```bash
gcc -o suite_testes main.c math_tests.c
./suite_testes
```
