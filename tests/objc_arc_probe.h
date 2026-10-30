#ifndef CEST_ARC_PROBE_H
#define CEST_ARC_PROBE_H
#ifdef __APPLE__
#import <Foundation/Foundation.h>
@interface CestARCProbe : NSObject @end
#else
#import <objc/runtime.h>
__attribute__((objc_root_class))
@interface CestARCProbe { Class isa; }
+ (id)new;
#if !__has_feature(objc_arc)
- (id)retain;
- (void)release;
- (id)autorelease;
- (void)dealloc;
#endif
@end
#endif
extern int cest_arc_probe_deallocations;
#endif
