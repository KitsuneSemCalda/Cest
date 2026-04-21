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

### Versões de Linguagem

| Linguagem | Versões | Notas |
|---------|:-------|:------|
| **C** | C89, C99, C11, C17, C23 | Suporte completo, macros modernos disponíveis |
| **C++** | C++11, C++17, C++20, C++23 | Type handling baseado em templates |
| **ObjC** | ARC & non-ARC | Anotações de nullability suportadas |

### Compatibilidade de Compiladores

| Compilador | Flags |
|:----------|:-----|
| **GCC** | `-std=c11 -Wall -Wextra` |
| **Clang** | `-std=c11 -Wall -Wextra` |
| **MSVC** | `/std:c11` (parcial) |

## Por que Cest?

- **Header-only**: Apenas um arquivo `cest.h` para incluir
- **Sintaxe expressiva**: API fluente inspirada em Jest
- **Sem dependências**: Não requer bibliotecas externas
- **Multi-linguagem**: Funciona com C, C++, Objective-C e Objective-C++
- **Suporte moderno**: C11/C17/C23, C++17/C++20/C++23, ObjC com ARC
- **Suporte a hooks**: beforeAll/afterAll/beforeEach/afterEach
- **Benchmarking integrado**: Medição simples de performance
- **Compatível com sanitizers**: Funciona perfeitamente com ASan, TSan, MSan
- **Saída limpa**: Supressão automática de warnings do compilador
- **Extensível**: Crie matchers customizados para asserções específicas de domínio
- **Execução flexível**: Modificadores Skip/Only para testes focados
- **Pronto para CI/CD**: Formatos de saída JUnit XML e JSON

## Funcionalidades

### Testes
- **Sintaxe Describe/It** - Estrutura de testes familiar como Jest
- **Múltiplos matchers** - Operações de igualdade, comparação, string, array
- **Hooks de teste** - Setup/teardown com beforeAll, afterAll, beforeEach, afterEach
- **Matchers customizados** - Estenda Cest com asserções específicas de domínio
- **Seleção de testes** - Modificadores Skip/Only para desenvolvimento focado

### Qualidade e Segurança
- **Detecção de vazamento de memória** - Integração com ASan, MSan, Valgrind
- **Suporte a sanitizers** - Funciona com todos os sanitizers principais (ASan, TSan, MSan, UBSan)
- **Benchmarking** - Ferramentas integradas de medição de performance
- **Testes thread-safe** - Suporte para testes concorrentes

### DevOps e CI
- **Múltiplos formatos de saída** - Terminal, JUnit XML, JSON
- **Detecção de ambiente CI** - Auto-detecção de GitHub Actions, GitLab CI, Jenkins
- **Códigos de saída** - Status de saída padrão para integração de scripts
- **Integração Valgrind** - Workflows de debug de memória

## Documentação

### Começando
- [Instalação](./installation.md)
- [Conceitos Básicos](./basics.md)
- [Exemplos](./examples.md)

### Conceitos Principais
- [Matchers](./matchers.md)
- [Tipos](./types.md)
- [Referência de API](./api.md)

### Tópicos Avançados
- [Matchers Customizados](./custom_matchers.md) - Estenda com asserções específicas de domínio
- [Skip e Only](./skip_only.md) - Seleção de testes e debug focado
- [Detecção de Vazamentos](./leak_detection.md) - Segurança de memória com ASan/Valgrind
- [Integração CI/CD](./ci_integration.md) - Setup para GitHub Actions, GitLab, Jenkins

### Referência
- [Limitações](./limitations.md)