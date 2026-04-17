#include "../../cest.h"
#include <string>
#include <regex>

int main() {
    describe("C++ Regex Matchers", {
        it("matches regex pattern", {
            std::string email = "test@example.com";
            std::regex pattern(R"(^[a-z]+@[a-z]+\.[a-z]+$)");
            expect(email).toMatch(pattern);
        });

        it("matches with std::string", {
            std::string s = "hello world";
            expect(s).toContain("world");
        });

        it("works with string equality", {
            std::string a = "hello";
            std::string b = "hello";
            expect(a).toEqual(b);
        });
    });

    return cest_result();
}
