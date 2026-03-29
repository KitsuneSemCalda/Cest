# 🧪 Cest

**Minimalist C Unit Testing Engine inspired by Jest and Gest.**

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Header Only](https://img.shields.io/badge/header--only-C11-orange.svg)](cest.h)

Cest is a lightweight, **header-only** testing framework for C that brings the expressive syntax of modern JavaScript and Go testing tools to the C language. It's designed for simplicity, ease of use, and immediate feedback.

---

## 🚀 Quick Start

```c
#include "cest.h"

int main() {
    describe("Math Suite", {
        it("sums numbers correctly", {
            expect(1 + 1).toEqual(2);
        });
    });

    return cest_result();
}
```

Compile and run:
```bash
gcc -o test_suite test_suite.c
./test_suite
```

---

## ✨ Features

- **No Linker Dependencies**: Zero external libraries (even `-lm` is not needed).
- **Modern Syntax**: Uses `describe`, `it`, and `expect` for highly readable tests.
- **Colorized Results**: Instant visual feedback in your terminal.
- **Single Header**: Just drop `cest.h` into your project and you are ready.
- **Type-Safe Matchers**: Leverage C11 `_Generic` for automatic type handling (int, double, string, pointers).

---

## 📚 Documentation

Detailed documentation is available in multiple languages:

- [🇧🇷 Português (docs/pt-br)](docs/pt-br/README.md)
- [🇺🇸 English (docs/us-en)](docs/us-en/README.md)

---

## 🔍 Examples

Check out more examples in the [examples/](examples/) directory:

- [Basic Usage](examples/basic.c)
- [Floating Point & Strings](examples/advanced.c)

---

## 📄 License

This project is licensed under the [MIT License](LICENSE).
