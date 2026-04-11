#include "../cest.h"

int main() {
    describe("Leak Detection Test", {
        it("should detect leaks", {
            void* ptr = cest_malloc(100);
            (void)ptr; // leak
            expect(1).toEqual(1);
        });
    });

    return cest_result();
}