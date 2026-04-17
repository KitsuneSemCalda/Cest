#include "../../cest.h"

// Intentionally crashes in a hook to test diagnostics
void crashing_hook(void) {
    volatile int* p = NULL;
    *p = 99;
}

int main(int argc, char* argv[]) {
    cest_init(argc, argv);

    beforeEach(crashing_hook);

    describe("Hook Crash Diagnostics", {
        it("should show hook name when beforeEach crashes", {
            expect(1).toEqual(1);
        });
    });

    return cest_result();
}
