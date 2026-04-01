# Instalação

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

## Usando o Makefile

```bash
make              # Compila todos os exemplos para build/
make run          # Compila e executa todos os exemplos
make clean        # Remove o diretório build/
make objc-deps   # Verifica/instala dependências ObjC
```

## Início Rápido

```c
#include "cest.h"

int main() {
    describe("Meus Testes", {
        it("deve somar", {
            expect(2 + 2).toBe(4);
        });
    });
    return cest_result();
}
```

Compile e execute:

```bash
gcc -o teste teste.c && ./teste
```