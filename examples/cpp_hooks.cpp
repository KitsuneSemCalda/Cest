#include "../cest.h"
#include <iostream>

int setup_count = 0;

void setup() {
    setup_count++;
    std::cout << "C++ Setup called " << setup_count << " times" << std::endl;
}

int main() {
    beforeAll(setup);

    describe("C++ Hooks Test", {
        it("should run after setup", {
            expect(setup_count).toEqual(1);
        });

        it("should increment", {
            setup_count++;
            expect(setup_count).toEqual(2);
        });
    });

    return cest_result();
}