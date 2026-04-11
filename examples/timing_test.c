#include "../cest.h"
#include <unistd.h>

int main() {
    describe("Timing Test", {
        it("should show timing for slow test", {
            volatile int sum = 0;
            for(int i = 0; i < 1000000; i++) sum += i;
            expect(sum > 0).toBeTruthy();
        });
    });

    return cest_result();
}