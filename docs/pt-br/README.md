# Cest (Portugues)

**Framework de testes unitarios minimalista inspirado no Jest e Gest.**

Cest e um framework de testes leve e **header-only** para C e linguagens relacionadas. Ele traz a sintaxe expressiva de ferramentas modernas de teste JavaScript e Go para linguagens da familia C.

## Linguagens Suportadas

| Linguagem | Extensao | Compilador | Suporte |
|-----------|----------|------------|:--------:|
| **C** | `.c` | `gcc`, `clang` | Sim |
| **C++** | `.cpp` | `g++`, `clang++` | Sim |
| **Objective-C** | `.m` | `clang -x objective-c` | Sim |
| **Objective-C++** | `.mm` | `clang++ -x objective-c++` | Sim |

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
make              # Compila todos os exemplos para build/
make run          # Compila e executa todos os exemplos
make clean        # Remove o diretorio build/
make objc-deps   # Verifica/instala dependencias ObjC
```

## Matchers Disponiveis

| Matcher | Descricao | Tipos Suportados |
|:---|:---|:---|
| `toBe(x)` / `toEqual(x)` | Verifica igualdade de valor ou identidade | int, double, string, pointer, bool, id |
| `toBeTruthy()` | Verifica se o valor e "verdadeiro" | int, double, string, pointer, bool, id |
| `toBeFalsy()` | Verifica se o valor e "falso" ou nulo | int, double, string, pointer, bool, id |
| `toBeNull()` | Verifica se um ponteiro e `NULL` ou `nil` | pointer, id |
| `toBeGreaterThan(x)` | Verifica se o valor e maior que `x` | int, double |
| `toBeLessThan(x)` | Verifica se o valor e menor que `x` | int, double |
| `toContain(substring)` | Verifica se uma string contem uma sub-string | string |
| `toBeCloseTo(val, prec)` | Compara numeros decimais com precisao especifica | double |

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
    describe("Strings", {
        it("suporta std::string", {
            std::string s = "hello world";
            expect(s).toEqual("hello world");
        });
    });

    return cest_result();
}
```

### Objective-C

```objc
#import "cest.h"

int main() {
    describe("Objective-C", {
        it("trabalha com id", {
            id obj = (id)0x1234;
            expect(obj).toBeTruthy();
        });

        it("trabalha com nil", {
            id nil_obj = nil;
            expect(nil_obj).toBeNull();
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
    describe("ObjC++", {
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

1. **Inicializacao com chaves**: Nao utilize inicializadores com chaves (`int arr[] = {1, 2, 3};`) dentro de blocos `it()`. Declare arrays fora dos blocos.

2. **Tipos especiais**: Alguns tipos como `size_t`, `std::shared_ptr`, e `std::unique_ptr` nao tem suporte nativo. Use casts para tipos basicos.

3. **Structs**: Para comparacao de structs, use funcoes auxiliares com `toBeTruthy()`:

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

## Multiplos Arquivos de Teste

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

```bash
gcc -o suite_testes main.c math_tests.c
./suite_testes
```

## Licenca

Este projeto e licenciado sob a [Licenca BSD-3 Clause](LICENSE).
