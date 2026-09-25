// Self-test for the --junit writer: it must count test cases, not
// assertions, and must include filtered-out tests as <skipped/> cases.
// This example deliberately fails one of its own tests to check that a
// failing testcase is counted once no matter how many assertions inside it
// failed - so its own cest_result() is intentionally not returned from
// main(); only the JUnit file it produces is graded, below.
#include "../../cest.h"
#include <stdio.h>
#include <string.h>

static const char* junit_path = "build/junit_report_self_test.xml";

int main() {
    _cest_junit_output = junit_path;
    _cest_global_stats.filter_pattern = "keep";

    describe("JUnit report accuracy", {
        it("keep: two assertions in one passing test", {
            expect(1 + 1).toEqual(2);
            expect(2 + 2).toEqual(4);
        });

        it("keep: two assertions in one failing test", {
            expect(1).toEqual(1);
            expect(1).toEqual(2); // deliberately wrong
        });

        it("dropped by filter", {
            expect(1).toEqual(1);
        });
    });

    cest_result(); // writes junit_path; this run's own pass/fail is not the point

    FILE* f = fopen(junit_path, "r");
    if (!f) {
        fprintf(stderr, "FAIL: could not open %s\n", junit_path);
        return 1;
    }
    char buf[8192];
    size_t n = fread(buf, 1, sizeof(buf) - 1, f);
    buf[n] = '\0';
    fclose(f);

    int ok = 1;

    // 4 assertions ran across 2 executed test cases, plus 1 filtered out:
    // tests must be 3 (test cases), never 4 (assertions).
    if (!strstr(buf, "tests=\"3\"")) {
        fprintf(stderr, "FAIL: expected tests=\"3\":\n%s\n", buf);
        ok = 0;
    }
    // One test case had a failing assertion; failures counts that ONE test
    // case, not the assertions inside it.
    if (!strstr(buf, "failures=\"1\"")) {
        fprintf(stderr, "FAIL: expected failures=\"1\":\n%s\n", buf);
        ok = 0;
    }
    if (!strstr(buf, "skipped=\"1\"")) {
        fprintf(stderr, "FAIL: expected skipped=\"1\":\n%s\n", buf);
        ok = 0;
    }
    int testcase_count = 0;
    for (const char* p = buf; (p = strstr(p, "<testcase")) != NULL; p += 9) testcase_count++;
    if (testcase_count != 3) {
        fprintf(stderr, "FAIL: expected 3 <testcase> elements (2 run + 1 skipped), got %d:\n%s\n", testcase_count, buf);
        ok = 0;
    }
    if (!strstr(buf, "<skipped/>")) {
        fprintf(stderr, "FAIL: expected a <skipped/> element for the filtered test:\n%s\n", buf);
        ok = 0;
    }
    if (!strstr(buf, "<failure message=\"assertion failed\"></failure>")) {
        fprintf(stderr, "FAIL: expected a <failure> element for the failing test:\n%s\n", buf);
        ok = 0;
    }

    if (ok) {
        printf("OK: JUnit report counts test cases, not assertions, and includes the filtered one\n");
        return 0;
    }
    return 1;
}
