#include "../../cest.h"
#include <regex.h>

// Initializers contain commas, so they live outside the describe() macro.
static int same_a[3] = {1, 2, 3};
static int same_b[3] = {1, 2, 3};
static int diff[3]   = {1, 2, 4};

int main() {
    describe("String Matchers", {
        it("contains substring", {
            const char* str = "hello world";
            expect(str).toContain("world");
        });

        it("does not contain substring", {
            const char* str = "hello world";
            // Cest has no negated matcher yet (see TODO.md), so falling back
            // to a plain C check plus toBeNull() to express "does not contain".
            // Note the (void*) cast: cest_value() maps char*/const char* to a
            // string value even when NULL, so toBeNull() needs a pointer type
            // to match against (see the same pattern in examples/c/basic.c).
            expect((void*)strstr(str, "foo")).toBeNull();
        });
    });

    describe("Numeric Matchers", {
        it("checks greater than", {
            expect(10).toBeGreaterThan(5);
        });

        it("checks less than", {
            expect(3).toBeLessThan(7);
        });

        it("checks close to", {
            expect(3.14159).toBeCloseTo(3.14, 0.01);
        });

        it("checks close to with an integer actual", {
            expect(5).toBeCloseTo(5.0, 0.1);
        });
    });

    describe("Array Matchers", {
        it("equal int arrays match", {
            expect_array(same_a, 3).toEqualArray(same_b, 3);
        });

        it("arrays differing past the first bytes do not match", {
            expect(match_eq(cest_array(same_a, 3), cest_array(diff, 3), NULL)).toBe(0);
        });
    });

    describe("Boolean Matchers", {
        it("checks truthy", {
            int val = 42;
            expect(val).toBeTruthy();
        });

        it("checks falsy", {
            int val = 0;
            expect(val).toBeFalsy();
        });
    });

    return cest_result();
}
