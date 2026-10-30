/* Compile this runtime-only fixture without ARC. No application framework is
 * required on Linux/libobjc2; Apple builds use the system NSObject. */
#import "objc_arc_probe.h"
#ifndef __APPLE__
#import <objc/objc-arc.h>
#endif
int cest_arc_probe_deallocations;
@implementation CestARCProbe
#ifndef __APPLE__
+ (id)new { return class_createInstance(self, 0); }
- (id)retain { return objc_retain_fast_np(self); }
- (void)release { objc_release_fast_np(self); }
- (id)autorelease { return objc_autorelease(self); }
- (void)_ARCCompliantRetainRelease {}
#endif
- (void)dealloc {
    ++cest_arc_probe_deallocations;
#ifdef __APPLE__
    [super dealloc];
#else
    object_dispose(self);
#endif
}
@end
