#include "cest.h"

int global_value = 0;

int setup(void) {
    global_value = 10;
    return 0;
}

int teardown(void) {
    global_value = 0;
    return 0;
}

int main() {
    beforeEach(setup);
    afterEach(teardown);

    describe("Fixture Suite", {
        it("has setup value", {
            expect(global_value).toBe(10);
        });

        it("value persists in test", {
            expect(global_value).toBeGreaterThan(0);
        });
    });

    return cest_result();
}
