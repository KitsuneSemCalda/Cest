# Cest - Overview

**Minimalist unit testing framework inspired by Jest and Gest.**

Cest is a lightweight, **header-only** testing framework for C and related languages. It brings the expressive syntax of modern JavaScript and Go testing tools to C-family languages.

## Demo

![Cest Demo](../tapes/basic_c.gif)

## Supported Languages

| Language | Extension | Compiler | Support |
|----------|-----------|----------|:--------:|
| **C** | `.c` | `gcc`, `clang` | Yes |
| **C++** | `.cpp` | `g++`, `clang++` | Yes |
| **Objective-C** | `.m` | `clang -x objective-c` | Yes |
| **Objective-C++** | `.mm` | `clang++ -x objective-c++` | Yes |

## Why Cest?

- **Header-only**: Just one `cest.h` file to include
- **Expressive syntax**: Fluent API inspired by Jest
- **No dependencies**: No external libraries required
- **Multi-language**: Works with C, C++, Objective-C, and Objective-C++
- **Hooks support**: beforeAll/afterAll/beforeEach/afterEach
- **Built-in benchmarking**: Simple performance measurement
- **Sanitizer compatible**: Works seamlessly with ASan, TSan, MSan

## Documentation

- [Installation](./installation.md)
- [Basics](./basics.md)
- [Matchers](./matchers.md)
- [Types](./types.md)
- [API](./api.md)
- [Examples](./examples.md)
- [Limitations](./limitations.md)