#include "../cest.h"

int setup_count = 0;

void setup() {
    setup_count++;
    printf("Setup called %d times\n", setup_count);
}

int main(int argc, char* argv[]) {
    cest_init(argc, argv);

    beforeAll(setup);

    describe("Benchmark and Hooks Test", {
        it("should run after setup", {
            expect(setup_count).toEqual(1);
        });

        bench("simple addition", {
            volatile int x = 1 + 1;
            (void)x;
        });
    });

    return cest_result();
}