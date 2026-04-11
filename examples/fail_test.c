#include "../cest.h"

#include "../cest.h"

int main() {
    describe("Error Messages Test", {
        it("should show string diff", {
            expect("hello").toEqual("helo");
        });
        it("should show double diff", {
            expect(3.14).toEqual(3.15);
        });
    });

    return cest_result();
}