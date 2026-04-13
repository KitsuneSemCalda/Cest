#import "../cest.h"
#import <objc/runtime.h>

// Root class definition for bare ObjC
__attribute__((objc_root_class))
@interface LeakTestClass
+ (Class)class;
@end

@implementation LeakTestClass
+ (Class)class { return self; }
@end

int main() {
    Class leak_test_class = [LeakTestClass class];

    describe("Objective-C Memory Leak Tests", {
        it("object allocation works correctly (batch)", {
            int leak_allocations = 0;
            
            for (int i = 0; i < 1000; i++) {
                void *obj = class_createInstance(leak_test_class, 0);
                if (obj != NULL) {
                    leak_allocations++;
                    free(obj);
                }
            }
            
            expect(leak_allocations).toBe(1000);
            printf("  -> 1000 allocations completed\n");
        });
    });

    describe("Objective-C Basic", {
        test("simple math", {
            expect(2 + 2).toBe(4);
        });
    });

    return cest_result();
}
