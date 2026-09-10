#define CEST_THREAD_SAFE
#include "../../cest.h"
#include <pthread.h>

// ============================================================================
// Thread Safety Example
// ============================================================================
// With CEST_THREAD_SAFE defined, the shared stats counters are protected by
// a mutex and the per-assertion context (_cest_ctx) becomes thread-local, so
// multiple threads can each run their own expect()...toX() chains without
// corrupting one another or losing counter updates.
//
// Build with: gcc -DCEST_THREAD_SAFE -pthread examples/features/thread_safety.c -o test
// For extra confidence, rebuild with -fsanitize=thread to confirm there is
// no data race on the shared state.

#define THREAD_COUNT 8
#define ITERATIONS_PER_THREAD 500

void* worker(void* arg) {
    (void)arg;
    for (int i = 0; i < ITERATIONS_PER_THREAD; i++) {
        expect(1 + 1).toEqual(2);
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    cest_init(argc, argv);

    pthread_t threads[THREAD_COUNT];
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_create(&threads[i], NULL, worker, NULL);
    }
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }

    describe("Thread Safety", {
        it("keeps the shared pass counter consistent across threads", {
            // If CEST_LOCK()/CEST_UNLOCK() were no-ops (or the mutex missing),
            // concurrent increments to _cest_global_stats.passed would race
            // and this count would come up short.
            expect(_cest_global_stats.passed).toEqual(THREAD_COUNT * ITERATIONS_PER_THREAD);
        });

        it("recorded no failures", {
            expect(_cest_global_stats.failed).toEqual(0);
        });
    });

    return cest_result();
}
