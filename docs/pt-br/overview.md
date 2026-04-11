# Cest - Introdução

**Framework de testes unitários minimalista inspirado no Jest e Gest.**

Cest é um framework de testes leve e **header-only** para C e linguagens relacionadas. Ele traz a sintaxe expressiva de ferramentas modernas de teste JavaScript e Go para linguagens da família C.

## Demonstração

![Cest Demo](../tapes/basic_c.gif)

## Linguagens Suportadas

| Linguagem | Extensão | Compilador | Suporte |
|-----------|----------|------------|:--------:|
| **C** | `.c` | `gcc`, `clang` | Sim |
| **C++** | `.cpp` | `g++`, `clang++` | Sim |
| **Objective-C** | `.m` | `clang -x objective-c` | Sim |
| **Objective-C++** | `.mm` | `clang++ -x objective-c++` | Sim |

## Por que Cest?

- **Header-only**: Apenas um arquivo `cest.h` para incluir
- **Sintaxe expressiva**:API fluente inspirada em Jest
- **Sem dependências**: Não requer bibliotecas externas
- **Multi-linguagem**: Funciona com C, C++, Objective-C e Objective-C++
- **Suporte a hooks**: beforeAll/afterAll/beforeEach/afterEach
- **Benchmarking integrado**: Medição simples de performance
- **Compatível com sanitizers**: Funciona perfeitamente com ASan, TSan, MSan

## Documentação

- [Instalação](./installation.md)
- [Conceitos Básicos](./basics.md)
- [Matchers](./matchers.md)
- [Tipos](./types.md)
- [API](./api.md)
- [Exemplos](./examples.md)
- [Limitações](./limitations.md)