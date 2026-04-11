#include "../cest.h"

int main(int argc, char* argv[]) {
    cest_init(argc, argv);

    describe("New Matchers Test", {
        it("should test toBeInRange", {
            expect(5).toBeInRange(1, 10);
            expect(3.14).toBeInRange(3.0, 4.0);
        });
        it("should test toStartWith", {
            expect("hello world").toStartWith("hello");
        });
        it("should test toEndWith", {
            expect("hello world").toEndWith("world");
        });
    });

    return cest_result();
}