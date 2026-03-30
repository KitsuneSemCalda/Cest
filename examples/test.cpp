#include "cest.h"

int main() {
    describe("C++ Basic", {
        test("simple math", {
            expect(2 + 2).toBe(4);
        });
    });
    return cest_result();
}
