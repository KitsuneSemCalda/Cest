# Cest - Overview

**Minimalist unit testing framework inspired by Jest and Gest.**

Cest is a lightweight, **header-only** testing framework for C and related languages. It brings the expressive syntax of modern JavaScript and Go testing tools to C-family languages.

## Demo

![Cest Demo](../tapes/basic_c.gif)

## Supported Languages

| Language | Extension | Compiler | Support |
|:---------|----------|----------|:--------:|
| **C** | `.c` | `gcc`, `clang` | Yes |
| **C++** | `.cpp` | `g++`, `clang++` | Yes |
| **Objective-C** | `.m` | `clang -x objective-c` | Yes |
| **Objective-C++** | `.mm` | `clang++ -x objective-c++` | Yes |

### Language Versions

| Language | Versions | Notes |
|:---------|:--------|:------|
| **C** | C89, C99, C11, C17, C23 | Full support, modern macros available |
| **C++** | C++11, C++17, C++20, C++23 | Template-based type handling |
| **ObjC** | ARC & non-ARC | Nullability annotations supported |

### Compiler Compatibility

| Compiler | Flags |
|:---------|:-----|
| **GCC** | `-std=c11 -Wall -Wextra` |
| **Clang** | `-std=c11 -Wall -Wextra` |
| **MSVC** | `/std:c11` (partial) |

## Why Cest?

- **Header-only**: Just one `cest.h` file to include
- **Expressive syntax**: Fluent API inspired by Jest
- **No dependencies**: No external libraries required
- **Multi-language**: Works with C, C++, Objective-C, and Objective-C++
- **Modern language support**: C11/C17/C23, C++17/C++20/C++23, ObjC with ARC
- **Hooks support**: beforeAll/afterAll/beforeEach/afterEach
- **Built-in benchmarking**: Simple performance measurement
- **Sanitizer compatible**: Works seamlessly with ASan, TSan, MSan
- **Clean output**: Automatic compiler warning suppression
- **Extensible**: Create custom matchers for domain-specific assertions
- **Flexible execution**: Skip/Only modifiers for focused testing
- **CI/CD ready**: JUnit XML and JSON output formats

## Features

### Testing
- **Describe/It syntax** - Familiar test structure like Jest
- **Multiple matchers** - Equality, comparison, string, array operations
- **Test hooks** - Setup/teardown with beforeAll, afterAll, beforeEach, afterEach
- **Custom matchers** - Extend Cest with domain-specific assertions
- **Test selection** - Skip/Only modifiers for focused development

### Quality & Safety
- **Memory leak detection** - Integrated with ASan, MSan, Valgrind
- **Sanitizer support** - Works with all major sanitizers (ASan, TSan, MSan, UBSan)
- **Benchmarking** - Built-in performance measurement tools
- **Thread-safe testing** - Support for concurrent tests

### DevOps & CI
- **Multiple output formats** - Terminal, JUnit XML, JSON
- **CI environment detection** - Auto-detection of GitHub Actions, GitLab CI, Jenkins
- **Exit codes** - Standard exit status for script integration
- **Valgrind integration** - Memory debugging workflows

## Documentation

### Getting Started
- [Installation](./installation.md)
- [Basics](./basics.md)
- [Examples](./examples.md)

### Core Concepts
- [Matchers](./matchers.md)
- [Types](./types.md)
- [API Reference](./api.md)

### Advanced Topics
- [Custom Matchers](./custom_matchers.md) - Extend with domain-specific assertions
- [Skip and Only](./skip_only.md) - Test selection and focused debugging
- [Leak Detection](./leak_detection.md) - Memory safety with ASan/Valgrind
- [CI/CD Integration](./ci_integration.md) - Setup for GitHub Actions, GitLab, Jenkins

### Reference
- [Limitations](./limitations.md)