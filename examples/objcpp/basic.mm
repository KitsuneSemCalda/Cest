#import "../../cest.h"
#import <string>

static int add_ints(int a, int b) { return a + b; }

int main() {
    describe("Objective-C++ Basic Types", {
        it("should handle integers", {
            expect(2 + 2).toBe(4);
            expect(10 - 3).toEqual(7);
        });

        it("should handle floating point", {
            expect(3.14 + 0.86).toBeCloseTo(4.0, 0.001);
        });

        it("should handle integers as bool", {
            expect(1).toBeTruthy();
            expect(0).toBeFalsy();
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

        it("should work with functions", {
            expect(add_ints(2, 3)).toBe(5);
            expect(add_ints(-1, 1)).toEqual(0);
        });
    });

    describe("Objective-C++ id Type", {
        it("should handle id type", {
            id obj1 = (id)0x1234;
            id obj2 = (id)0x1234;
            expect(obj1).toBe(obj2);
        });

        it("should handle nil", {
            id nil_obj = nil;
            expect(nil_obj).toBeNull();
            expect(nil_obj).toBeFalsy();
        });

        it("should mix ObjC and C++ types", {
            id obj = (id)0x5678;
            std::string s = "mixed world";
            expect(obj).toBeTruthy();
            expect(s).toContain("world");
        });
    });

    return cest_result();
}
