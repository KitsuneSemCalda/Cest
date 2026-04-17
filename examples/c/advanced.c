#include "../../cest.h"

int main() {
    describe("Advanced String Matchers", {
        it("should find substrings using toContain", {
            expect("Cest is a header-only library").toContain("header-only");
            expect("Hello World").toContain("World");
        });

        it("should check string identity with toEqual", {
            expect("Apple").toEqual("Apple");
        });
    });

    describe("Floating Point Math (No -lm needed)", {
        it("should handle double precision errors", {
            // 0.1 + 0.2 is actually 0.30000000000000004
            expect(0.1 + 0.2).toBeCloseTo(0.3, 0.000001);
        });

        it("should compare doubles directly", {
            expect(3.14).toEqual(3.14);
        });
    });

    describe("Pointers and Memory", {
        it("should handle NULL pointer checks", {
            void* my_ptr = NULL;
            expect(my_ptr).toBeNull();
        });

        it("should compare pointer addresses", {
            int x = 10;
            int* px = &x;
            expect(px).toEqual(&x);
        });
    });

    return cest_result();
}
