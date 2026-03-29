#include "../cest.h"

int main() {
    describe("Basic Integers", {
        it("should perform simple math", {
            expect(2 + 2).toBe(4);
            expect(10 * 5).toEqual(50);
        });

        it("should handle comparisons", {
            expect(100).toBeGreaterThan(50);
            expect(10).toBeLessThan(20);
        });
    });

    describe("Boolean Logic", {
        it("should detect truthy values", {
            expect(true).toBeTruthy();
            expect(1).toBeTruthy();
        });

        it("should detect falsy values", {
            expect(false).toBeFalsy();
            expect(0).toBeFalsy();
            expect(NULL).toBeFalsy();
        });
    });

    return cest_result();
}
