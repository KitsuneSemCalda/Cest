#include "cest.h"

int main() {
    describe("String Matchers", {
        it("should find substrings", {
            expect("Cest is awesome").toContain("awesome");
            expect("Jest, Gest, Cest").toContain("Gest");
        });
    });

    describe("Floating Point", {
        it("should handle floating point math", {
            expect(0.1 + 0.2).toBeCloseTo(0.3, 0.000001);
        });
    });

    describe("Complex Examples", {
        it("should handle various types in a single suite", {
            expect(100).toBe(100);
            expect("apple").toContain("app");
            expect(NULL).toBeNull();
            expect(true).toBeTruthy();
        });
    });

    return cest_result();
}
