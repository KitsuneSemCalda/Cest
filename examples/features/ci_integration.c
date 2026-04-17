#include "../../cest.h"

// ============================================================================
// CI/CD Integration Example
// ============================================================================
// Cest supports multiple output formats for CI/CD pipelines:
//
// 1. JUnit XML format (for Jenkins, GitHub Actions, GitLab CI)
//    ./test --junit report.xml
//
// 2. JSON format (for custom processing)
//    ./test --json report.json
//
// 3. Terminal output (default, colorized)
//    ./test
//
// Compile: gcc -o test examples/ci_integration.c
//
// Run with outputs:
//    ./test --junit junit-report.xml --json json-report.json
//
// GitHub Actions example (.github/workflows/test.yml):
// - name: Run tests
//   run: ./test --junit test-results.xml
// - name: Publish test results
//   if: always()
//   uses: dorny/test-reporter@v1
//   with:
//     name: C Tests
//     path: test-results.xml
//     reporter: 'java-junit'

int main(int argc, char* argv[]) {
    cest_init(argc, argv);

    describe("Calculator Functions", {
        it("adds two positive numbers", {
            expect(2 + 3).toEqual(5);
        });

        it("adds negative numbers", {
            expect(-5 + 3).toEqual(-2);
        });

        it("subtracts numbers", {
            expect(10 - 3).toEqual(7);
        });

        it("multiplies numbers", {
            expect(4 * 5).toEqual(20);
        });

        it("handles zero", {
            expect(0 + 5).toEqual(5);
            expect(5 * 0).toEqual(0);
        });
    });

    describe("String Functions", {
        it("compares equal strings", {
            expect("hello").toEqual("hello");
        });

        it("finds substrings", {
            expect("hello world").toContain("world");
            expect("test").toContain("es");
        });

        it("starts with prefix", {
            expect("hello").toStartWith("hel");
            expect("testing").toStartWith("test");
        });

        it("ends with suffix", {
            expect("hello").toEndWith("lo");
            expect("testing").toEndWith("ing");
        });
    });

    describe("Pointer Handling", {
        it("detects null pointers", {
            int* p = NULL;
            expect(p).toBeNull();
        });

        it("detects valid pointers", {
            int x = 42;
            int* p = &x;
            expect(p).toBeTruthy();
        });
    });

    describe("Boolean Logic", {
        it("evaluates truthy values", {
            expect(1).toBeTruthy();
            expect(42).toBeTruthy();
            expect(-1).toBeTruthy();
        });

        it("evaluates falsy values", {
            expect(0).toBeFalsy();
            expect((void*)NULL).toBeFalsy();
        });
    });

    return cest_result();
}
