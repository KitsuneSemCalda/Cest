#include "../../cest.h"

void run_string_tests() {
    describe_file({
        describe("String Tests", {
            it("should verify string logic in a separate file", {
                expect("Hello").toEqual("Hello");
                expect("Cest MultiFile Framework").toContain("MultiFile");
            });
        });
    });
}