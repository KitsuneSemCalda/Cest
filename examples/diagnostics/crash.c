#include "../../cest.h"

// Intentionally triggers SIGSEGV to test crash diagnostics
int main(int argc, char* argv[]) {
    cest_init(argc, argv);

    describe("Signal Handler Diagnostics", {
        it("should pass a normal test before the crash", {
            expect(1 + 1).toEqual(2);
        });

        it("should detect SIGSEGV and show test name", {
            // This will crash — the signal handler should print:
            //   ✕ CRASH: SIGSEGV (Segmentation Fault) during test: should detect SIGSEGV and show test name
            volatile int* bad_ptr = NULL;
            *bad_ptr = 42;
            // Should never reach here
            expect(1).toEqual(1);
        });

        // This test should NOT run because the process exits on crash
        it("should not run after crash", {
            expect(1).toEqual(1);
        });
    });

    return cest_result();
}
