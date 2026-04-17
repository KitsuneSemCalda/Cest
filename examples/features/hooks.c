#include "../../cest.h"

int setup_counter = 0;
int teardown_counter = 0;

void my_before_each(void) {
    setup_counter++;
}

void my_after_each(void) {
    teardown_counter++;
}

int main() {
    beforeEach(my_before_each);
    afterEach(my_after_each);

    describe("Hooks Suite", {
        it("runs beforeEach before first test", {
            expect(setup_counter).toBe(1);
        });

        it("runs afterEach after first test", {
            expect(teardown_counter).toBe(1);
        });

        it("increment counters correctly", {
            expect(setup_counter).toBeGreaterThan(0);
            expect(teardown_counter).toBeGreaterThan(0);
        });
    });

    return cest_result();
}
