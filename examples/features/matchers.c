#include "../../cest.h"
#include <limits.h>
#include <math.h>
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
            expect(strstr(str, "foo")).toBeNull();
        });

        it("a NULL char* matches toBeNull() directly", {
            const char* missing = NULL;
            expect(missing).toBeNull();
        });

        it("a NULL char* is not the literal string \"NULL\"", {
            const char* missing = NULL;
            expect(match_eq(cest_value(missing), cest_value("NULL"), NULL)).toBe(0);
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

        it("NaN is never close to any finite value, per IEEE 754", {
            // Any comparison against NaN is false, including `NaN < precision`,
            // so toBeCloseTo must report NaN as failing to match a finite
            // target rather than accidentally treating the always-false
            // comparison as a pass.
            double actual = NAN;
            expect(match_eq(cest_value(actual), cest_value(0.0), NULL)).toBe(0);
        });

        it("NaN does not equal itself, per IEEE 754", {
            double a = NAN;
            double b = NAN;
            expect(match_eq(cest_value(a), cest_value(b), NULL)).toBe(0);
        });

        it("compares INT_MAX and INT_MIN without overflowing", {
            expect((long long)INT_MAX).toBeGreaterThan((long long)INT_MIN);
            expect((long long)INT_MIN).toBeLessThan((long long)INT_MAX);
        });

        it("compares LLONG_MAX and LLONG_MIN without overflowing", {
            // match_gt/match_lt compare a.as.i/b.as.i directly (no
            // subtraction), so the widest representable range must compare
            // correctly instead of wrapping around.
            expect(LLONG_MAX).toBeGreaterThan(LLONG_MIN);
            expect(LLONG_MIN).toBeLessThan(LLONG_MAX);
            expect(LLONG_MAX).toEqual(LLONG_MAX);
            expect(LLONG_MIN).toEqual(LLONG_MIN);
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
