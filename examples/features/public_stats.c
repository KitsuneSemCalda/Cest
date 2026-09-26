#include "../../cest.h"

// Regression test for the TODO.md item: "_cest_global_stats is documented as
// public, so give it a replacement (e.g. cest_stats()) and deprecate the old
// name first." cest_stats() must mirror _cest_global_stats exactly (it is a
// read-only copy of the same weak global), and the old name must keep
// working unchanged for anyone still using it directly.
int main(int argc, char* argv[]) {
    cest_init(argc, argv);

    describe("Public stats accessor", {
        it("starts at zero before any assertion in this TU has run", {
            cest_stats_t stats = cest_stats();
            // This is the first assertion in the process, so passed/failed
            // must both still be zero at the point cest_stats() is read.
            expect(stats.passed).toBe(0);
            expect(stats.failed).toBe(0);
        });

        it("reflects a passed assertion recorded by _cest_global_stats", {
            cest_stats_t before = cest_stats();
            expect(1 + 1).toBe(2);
            cest_stats_t after = cest_stats();

            expect(after.passed).toBe(before.passed + 1);
        });

        it("matches _cest_global_stats field for field", {
            cest_stats_t stats = cest_stats();
            expect(stats.passed).toBe(_cest_global_stats.passed);
            expect(stats.failed).toBe(_cest_global_stats.failed);
            expect(stats.skipped).toBe(_cest_global_stats.skipped);
        });
    });

    return cest_result();
}
