#include "../../cest.h"

int main() {
    describe("Leak Detection Test", {
        it("should detect leaks", {
            void* ptr = cest_malloc(100);
            (void)ptr; // leak
            expect(1).toEqual(1);
        });

        it("should handle 1000 allocations as a single test", {
            int success_count = 0;
            for (int i = 0; i < 1000; i++) {
                void* p = cest_malloc(16);
                if (p) {
                    success_count++;
                    cest_free(p);
                }
            }
            // Instead of 1000 expects, we use one to keep the test count sane
            expect(success_count).toBe(1000);
            printf("  -> 1000 allocations completed\n");
        });
    });

    return cest_result();
}