#include "cest.h"

int main(int argc, char* argv[]) {
    cest_init(argc, argv);

    describe("CLI Arguments Suite", {
        it("can parse arguments", {
            expect(argc).toBeGreaterThan(0);
        });
    });

    return cest_result();
}
