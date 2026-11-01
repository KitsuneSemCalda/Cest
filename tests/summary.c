#include "../cest.h"

int main(int argc, char **argv) {
    int negative = argc > 1 && strcmp(argv[1], "--negative") == 0;
    cest_init(argc, argv);
    test("loop", {
        for (int i = 0; i < 100; ++i) expect(i >= 0).toBeTruthy();
    });
    test("empty", { (void)0; });
    test("failure", {
        expect(!negative).toBeTruthy();
        expect(!negative).toBeTruthy();
    });
    return cest_result();
}
