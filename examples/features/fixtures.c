#include "../../cest.h"

int global_value = 0;

void setup(void) {
    global_value = 10;
}

void teardown(void) {
    global_value = 0;
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
