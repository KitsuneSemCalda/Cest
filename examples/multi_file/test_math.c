#include "../../cest.h"

void run_math_tests() {
    describe_file({
        describe("Math Tests", {
            it("should test basics in a separate translation unit", {
                expect(10 + 20).toEqual(30);
                expect(5.0 * 2.0).toBeCloseTo(10.0, 0.001);
            });
        });
    });
}