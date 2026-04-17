#include "../../cest.h"

int main(int argc, char* argv[]) {
    cest_init(argc, argv);

    // Using prefixed macros to avoid namespace collisions
    cest_describe("Prefixed API", {
        cest_it("should work with cest_ prefix", {
            cest_expect(2 + 2).toEqual(4);
            cest_expect("hello").toContain("ell");
        });

        cest_test("should also work with cest_test", {
            cest_expect(true).toBeTruthy();
        });
    });

    return cest_result();
}
