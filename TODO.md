# TODO

- [ ] Prefix internal symbols (`_cest_*`, `match_*`, `b_*`). `_cest_global_stats` is documented as public, so give it a replacement (e.g. `cest_stats()`) and deprecate the old name first.
- [ ] Reimplement `skip()`/`only()` if wanted (removed: it never compiled in C and never skipped anything).
- [ ] Reimplement per-test isolation (`fork`) if wanted (removed: never compiled).
- [ ] Self-tests for every matcher and edge cases (NULL, NaN, overflow), built on gcc, clang and MSVC in CI.
- [ ] Silence the C++ build warnings (`_POSIX_C_SOURCE` redefined, `-Wstrict-prototypes` pragma).
- [ ] `--list`, `--fail-fast`, TAP reporter.
