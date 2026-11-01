#!/bin/sh
set -eu
binary=$1
output=$(env -u CEST_POST_RUN "$binary")
printf '%s\n' "$output" | grep -F 'Tests: 3 passed, 0 failed, 0 skipped'
printf '%s\n' "$output" | grep -F 'Assertions: 102 passed, 0 failed'
code=0
output=$(env -u CEST_POST_RUN "$binary" --negative) || code=$?
test "$code" -eq 1
printf '%s\n' "$output" | grep -F 'Tests: 2 passed, 1 failed, 0 skipped'
printf '%s\n' "$output" | grep -F 'Assertions: 100 passed, 2 failed'
output=$(env -u CEST_POST_RUN "$binary" empty)
printf '%s\n' "$output" | grep -F 'Tests: 1 passed, 0 failed, 2 skipped'
printf '%s\n' "$output" | grep -F 'Assertions: 0 passed, 0 failed'
