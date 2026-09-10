#include "../../cest.h"
#include <regex.h>

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
