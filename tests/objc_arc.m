/* Standalone regression: this includes no FoxStep headers or implementation.
 * Compile this same source as Objective-C and Objective-C++, with and without
 * ARC, against libobjc2 (Linux) or the native Apple runtime. */
#import "objc_arc_probe.h"
#include "../cest.h"
int main(int argc, char **argv) {
    @autoreleasepool {
        if (argc == 2 && strcmp(argv[1], "--negative") == 0) {
            /* Exercises failure formatting (the id-to-void* diagnostic cast). */
            expect((id)nil).toBeTruthy();
            return cest_result();
        }
        describe("Objective-C object assertions", {
            it("supports nil, BOOL and object/pointer comparisons", {
                id object = [CestARCProbe new];
                BOOL yes = YES;
                BOOL no = NO;
                expect(object).toBeTruthy();
                expect((id)nil).toBeNull();
                expect(yes).toBeTruthy();
                expect(no).toBeFalsy();
                expect((id)nil).toEqual((const void *)NULL);
                expect((const void *)NULL).toEqual((id)nil);
#if !__has_feature(objc_arc)
                [object release];
#endif
            });
#if __has_feature(objc_arc)
            it("does not retain an object in the last assertion's global context", {
                __weak id weak;
                int before = cest_arc_probe_deallocations;
                @autoreleasepool {
                    id object = [CestARCProbe new];
                    weak = object;
                    expect(object).toBeTruthy();
                    object = nil;
                }
                expect(weak == nil).toBeTruthy();
                expect(cest_arc_probe_deallocations == before + 1).toBeTruthy();
            });
#endif
        });
        return cest_result();
    }
}
