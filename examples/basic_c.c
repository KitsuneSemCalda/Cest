#include "../cest.h"

static int add(int a, int b) { return a + b; }
static double divide(double a, double b) { return b != 0 ? a / b : 0; }

static int nums[] = {1, 2, 3};
static const char* strs[] = {"hello", "world"};

int main() {
    describe("C Basic Types", {
        it("should handle integers", {
            expect(2 + 2).toBe(4);
            expect(10 - 3).toEqual(7);
            expect(6 * 7).toEqual(42);
        });

        it("should handle floating point", {
            expect(3.14 + 0.86).toBeCloseTo(4.0, 0.001);
            expect(divide(10.0, 3.0)).toBeCloseTo(3.333, 0.001);
        });

        it("should handle booleans", {
            expect(true).toBeTruthy();
            expect(false).toBeFalsy();
            expect(1).toBeTruthy();
            expect(0).toBeFalsy();
        });

        it("should compare strings", {
            expect("hello").toEqual("hello");
            expect("world").toBe("world");
            expect("hello").toContain("ell");
        });

        it("should handle pointers", {
            int x = 42;
            expect(&x).toBeTruthy();
            expect((void*)NULL).toBeNull();
        });
    });

    describe("C Advanced", {
        it("should handle comparisons", {
            expect(10).toBeGreaterThan(5);
            expect(3).toBeLessThan(7);
        });

        it("should work with functions", {
            expect(add(2, 3)).toBe(5);
            expect(add(-1, 1)).toEqual(0);
        });

        it("should handle NULL pointers", {
            int* null_ptr = NULL;
            expect(null_ptr).toBeNull();
            expect(NULL).toBeFalsy();
        });

        it("should handle arrays", {
            expect(nums).toBeTruthy();
            expect(nums[0]).toEqual(1);
            expect(nums[1]).toEqual(2);
            expect(nums[2]).toEqual(3);
        });

        it("should handle string arrays", {
            expect(strs[0]).toEqual("hello");
            expect(strs[1]).toEqual("world");
        });
    });

    return cest_result();
}
