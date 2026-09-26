# TODO

- [x] Prefix internal symbols (`_cest_*`, `match_*`, `b_*`). `_cest_global_stats` is documented as public, so give it a replacement (e.g. `cest_stats()`) and deprecate the old name first. Done: `cest_stats()` added, `_cest_global_stats` kept for backward compatibility.
- [ ] Reimplement `skip()`/`only()` if wanted (removed: it never compiled in C and never skipped anything).
- [ ] Reimplement per-test isolation (`fork`) if wanted (removed: never compiled).
- [x] Self-tests for every matcher and edge cases (NULL, NaN, overflow), built on gcc, clang and MSVC in CI. NULL done in `137071d`; NaN and integer-boundary (`INT_MIN`/`INT_MAX`/`LLONG_MIN`/`LLONG_MAX`) cases added to `examples/features/matchers.c`.
- [ ] Silence the C++ build warnings (`_POSIX_C_SOURCE` redefined, `-Wstrict-prototypes` pragma).
- [ ] `--list`, `--fail-fast`, TAP reporter.
