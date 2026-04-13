#include "../cest.h"

// ============================================================================
// Skip and Only Test Modifiers
// ============================================================================
// The skip() and only() modifiers allow selective test execution.
// Compile with: -DCEST_ENABLE_SKIP
//
// Usage:
// - skip("suite name", { it("test", { ... }); }) - Skips this suite
// - only("suite name", { it("test", { ... }); }) - Runs ONLY this suite
//
// These are useful for:
// 1. Debugging - focus on one test while skipping others
// 2. Known issues - temporarily skip failing tests
// 3. Work in progress - test new features while skipping complete sections

int add(int a, int b) {
    return a + b;
}

int multiply(int a, int b) {
    return a * b;
}

int main(int argc, char* argv[]) {
    cest_init(argc, argv);

    describe("Math Suite - Skip and Only Example", {
        it("addition works", {
            expect(add(2, 2)).toEqual(4);
            expect(add(-1, 1)).toEqual(0);
        });

        it("multiplication works", {
            expect(multiply(3, 4)).toEqual(12);
            expect(multiply(-1, 5)).toEqual(-5);
        });

        it("handles edge cases", {
            expect(add(0, 0)).toEqual(0);
            expect(multiply(0, 100)).toEqual(0);
        });
    });

    describe("String Suite", {
        it("string comparison works", {
            expect("hello").toEqual("hello");
        });

        it("string contains works", {
            expect("hello world").toContain("world");
            expect("testing").toContain("test");
        });
    });

    // Uncomment 'only()' below to test selective execution
    // This demonstrates how to focus on specific test suites during development
    /*
    only("Focused Suite - Development Mode", {
        it("focused test that runs alone", {
            expect(42).toEqual(42);
        });

        it("all other suites will be skipped", {
            expect(true).toBeTruthy();
        });
    });
    */

    return cest_result();
}

