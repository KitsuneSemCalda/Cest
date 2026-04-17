#include "../../cest.h"
#include <vector>
#include <string>

int main() {
    describe("C++ Basic Types", {
        it("should handle integers", {
            expect(2 + 2).toBe(4);
            expect(10 - 3).toEqual(7);
        });

        it("should handle floating point", {
            expect(3.14 + 0.86).toBeCloseTo(4.0, 0.001);
        });

        it("should handle booleans", {
            expect(true).toBeTruthy();
            expect(false).toBeFalsy();
        });

        it("should handle std::string", {
            std::string s = "hello world";
            expect(s).toEqual("hello world");
            expect(s).toContain("world");
        });

        it("should handle pointers", {
            int x = 42;
            expect(&x).toBeTruthy();
            expect(static_cast<void*>(nullptr)).toBeNull();
        });
    });

    describe("C++ STL Containers", {
        it("should work with std::vector", {
            std::vector<int> v;
            v.push_back(1);
            v.push_back(2);
            v.push_back(3);
            expect(v.at(0)).toEqual(1);
            expect(v.at(1)).toEqual(2);
            expect(v.at(2)).toEqual(3);
        });

        it("should work with std::string concatenation", {
            std::string s1 = "hello";
            std::string s2 = "world";
            expect(s1 + " " + s2).toEqual("hello world");
        });
    });

    describe("C++ Type Inference", {
        it("should infer types correctly", {
            expect(3.14).toBeCloseTo(3.14, 0.001);
            expect(100).toEqual(100);
            expect(true).toBeTruthy();
        });
    });

    return cest_result();
}
