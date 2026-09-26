CC = gcc
CXX = g++
CFLAGS = -Wall -Wextra -std=c11
CXXFLAGS = -Wall -Wextra -std=c++11

OBJCFLAGS = -Wall -Wextra -x objective-c
OBJCCFLAGS = -Wall -Wextra -x objective-c++

GXX_VERSION := $(shell $(CXX) -dumpversion 2>/dev/null | cut -f1,2 -d. || echo "13")
GXX_MAJOR := $(shell $(CXX) -dumpversion 2>/dev/null | cut -d. -f1 || echo "13")

CLANG_INCLUDES = -I/usr/include/c++/$(GXX_VERSION) \
                 -I/usr/include/x86_64-linux-gnu/c++/$(GXX_VERSION) \
                 -I/usr/lib/gcc/x86_64-linux-gnu/$(GXX_MAJOR)/include \
                 -I/usr/lib/gcc/x86_64-linux-gnu/$(GXX_VERSION)/include

CLANG_EXISTS := $(shell command -v clang 2>/dev/null && echo "yes")

OBJC_CFLAGS :=
OBJC_LIBS :=
OBJC_AVAILABLE :=
OBJC_PKG_NAME :=

PKG_CONFIG := $(shell command -v pkg-config 2>/dev/null && echo "yes")

ifneq ($(PKG_CONFIG),)
  ifeq ($(shell $(PKG_CONFIG) --exists objc 2>/dev/null && echo "yes"),yes)
    OBJC_CFLAGS := $(shell $(PKG_CONFIG) --cflags objc)
    OBJC_LIBS := $(shell $(PKG_CONFIG) --libs objc)
    OBJC_AVAILABLE := yes
    OBJC_PKG_NAME := objc
  endif
  ifeq ($(OBJC_AVAILABLE),)
    ifeq ($(shell $(PKG_CONFIG) --exists objc-gnu 2>/dev/null && echo "yes"),yes)
      OBJC_CFLAGS := $(shell $(PKG_CONFIG) --cflags objc-gnu)
      OBJC_LIBS := $(shell $(PKG_CONFIG) --libs objc-gnu)
      OBJC_AVAILABLE := yes
      OBJC_PKG_NAME := objc-gnu
    endif
  endif
endif

ifneq ($(OBJC_AVAILABLE),yes)
  OBJC_LDFLAGS := $(shell ldconfig -p 2>/dev/null | grep -q libobjc && echo "-lobjc")
  ifneq ($(OBJC_LDFLAGS),)
    GCC_VERSION := $(shell gcc -dumpversion 2>/dev/null | cut -d. -f1)
    GCC_INCLUDE := /usr/lib/gcc/x86_64-linux-gnu/$(GCC_VERSION)/include
    OBJC_HEADERS := $(shell [ -f "$(GCC_INCLUDE)/objc/objc.h" ] && echo "yes")
    ifeq ($(OBJC_HEADERS),yes)
      OBJC_CFLAGS := -I$(GCC_INCLUDE)
      # gcc's libobjc lives in gcc's private lib dir, which gcc searches
      # automatically but clang does not — pass -L explicitly so OBJC_CC=clang
      # (the default whenever clang is installed) can still find -lobjc.
      OBJC_LIBS := -L/usr/lib/gcc/x86_64-linux-gnu/$(GCC_VERSION) -lobjc
      OBJC_AVAILABLE := yes
      OBJC_PKG_NAME := libobjc (gcc)
    else
      OBJC_HEADERS := $(shell [ -f /usr/include/objc/objc.h ] && echo "yes")
      ifeq ($(OBJC_HEADERS),yes)
        OBJC_CFLAGS := -I/usr/include
        OBJC_LIBS := -lobjc
        OBJC_AVAILABLE := yes
        OBJC_PKG_NAME := libobjc (system)
      endif
    endif
  endif
endif

ifeq ($(OBJC_AVAILABLE),yes)
  $(info Info: Objective-C support detected$(if $(OBJC_PKG_NAME), via $(OBJC_PKG_NAME)))
else
  $(info Warning: Objective-C runtime not found. ObjC/ObjCpp examples skipped.)
endif

ifdef CLANG_EXISTS
    OBJC_CC = clang
    OBJCC_CXX = clang++
else
    OBJC_CC = $(CC)
    OBJCC_CXX = $(CXX)
endif

OBJC_FLAGS = $(OBJCFLAGS) $(OBJC_CFLAGS)
OBJCC_FLAGS = $(OBJCCFLAGS) $(OBJC_CFLAGS) $(CLANG_INCLUDES)

BUILD_DIR = build

# Core Examples
EXAMPLES_C = $(BUILD_DIR)/c_basic $(BUILD_DIR)/c_advanced $(BUILD_DIR)/c_structs
EXAMPLES_CPP = $(BUILD_DIR)/cpp_basic $(BUILD_DIR)/cpp_hooks
EXAMPLES_MULTI = $(BUILD_DIR)/multi_test_suite

# Feature Examples
FEATURES = $(BUILD_DIR)/feat_hooks $(BUILD_DIR)/feat_fixtures $(BUILD_DIR)/feat_matchers \
           $(BUILD_DIR)/feat_leak_detection $(BUILD_DIR)/feat_prefix \
           $(BUILD_DIR)/feat_thread_safety $(BUILD_DIR)/feat_junit_report \
           $(BUILD_DIR)/feat_public_stats

# Diagnostic Examples
DIAGNOSTICS = $(BUILD_DIR)/diag_crash $(BUILD_DIR)/diag_hook_crash $(BUILD_DIR)/diag_fail

ALL_EXAMPLES = $(EXAMPLES_C) $(EXAMPLES_CPP) $(EXAMPLES_MULTI) $(FEATURES) $(DIAGNOSTICS)

ifeq ($(OBJC_AVAILABLE),yes)
  EXAMPLES_OBJC = $(BUILD_DIR)/objc_basic $(BUILD_DIR)/objc_advanced
  EXAMPLES_OBJCC = $(BUILD_DIR)/objcpp_basic
  ALL_EXAMPLES += $(EXAMPLES_OBJC) $(EXAMPLES_OBJCC)
endif

.PHONY: all run clean test objc-deps c-examples cpp-examples feature-examples diag-examples

all: $(BUILD_DIR) $(ALL_EXAMPLES)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# --- C Examples ---
$(BUILD_DIR)/c_basic: examples/c/basic.c cest.h
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/c_advanced: examples/c/advanced.c cest.h
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/c_structs: examples/c/structs.c cest.h
	$(CC) $(CFLAGS) $< -o $@

# --- C++ Examples ---
$(BUILD_DIR)/cpp_basic: examples/cpp/basic.cpp cest.h
	$(CXX) $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/cpp_hooks: examples/cpp/hooks.cpp cest.h
	$(CXX) $(CXXFLAGS) $< -o $@

# --- Objective-C Examples ---
$(BUILD_DIR)/objc_basic: examples/objc/basic.m cest.h
	$(OBJC_CC) $(OBJC_FLAGS) $< -o $@ $(OBJC_LIBS)

$(BUILD_DIR)/objc_advanced: examples/objc/advanced.m cest.h
	$(OBJC_CC) $(OBJC_FLAGS) $< -o $@ $(OBJC_LIBS)

$(BUILD_DIR)/objcpp_basic: examples/objcpp/basic.mm cest.h
	$(OBJCC_CXX) $(OBJCC_FLAGS) $< -o $@ $(OBJC_LIBS) -L/usr/lib/gcc/x86_64-linux-gnu/$(GXX_MAJOR) -lstdc++

# --- Multi-file Example ---
$(BUILD_DIR)/multi_test_suite: examples/multi_file/test_main.c examples/multi_file/test_math.c examples/multi_file/test_string.c cest.h
	$(CC) $(CFLAGS) examples/multi_file/test_main.c examples/multi_file/test_math.c examples/multi_file/test_string.c -o $@

# --- Feature Examples ---
$(BUILD_DIR)/feat_hooks: examples/features/hooks.c cest.h
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/feat_fixtures: examples/features/fixtures.c cest.h
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/feat_matchers: examples/features/matchers.c cest.h
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/feat_junit_report: examples/features/junit_report.c cest.h
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/feat_public_stats: examples/features/public_stats.c cest.h
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/feat_leak_detection: examples/features/leak_detection.c cest.h
	$(CC) $(CFLAGS) -DCEST_ENABLE_LEAK_DETECTION $< -o $@

$(BUILD_DIR)/feat_prefix: examples/features/prefix.c cest.h
	$(CC) $(CFLAGS) -DCEST_PREFIX $< -o $@

$(BUILD_DIR)/feat_thread_safety: examples/features/thread_safety.c cest.h
	$(CC) $(CFLAGS) -pthread $< -o $@

# --- Diagnostic Examples ---
$(BUILD_DIR)/diag_crash: examples/diagnostics/crash.c cest.h
	$(CC) $(CFLAGS) -DCEST_ENABLE_SIGNAL_HANDLER $< -o $@

$(BUILD_DIR)/diag_hook_crash: examples/diagnostics/hook_crash.c cest.h
	$(CC) $(CFLAGS) -DCEST_ENABLE_SIGNAL_HANDLER $< -o $@

$(BUILD_DIR)/diag_fail: examples/diagnostics/fail.c cest.h
	$(CC) $(CFLAGS) $< -o $@

# --- Runners ---
# Every example here is expected to report all its tests as passing (exit
# code 0 from cest_result()). The crash/fail examples under diagnostics/ are
# intentionally NOT run here — they demonstrate signal handling and failure
# reporting on purpose, so a non-zero/aborted exit from them is success, not
# failure; run them individually if you want to see that behavior:
#   ./build/diag_crash ; ./build/diag_hook_crash ; ./build/diag_fail
RUNNABLE_EXAMPLES = $(EXAMPLES_C) $(EXAMPLES_CPP) $(EXAMPLES_MULTI) $(FEATURES)
ifeq ($(OBJC_AVAILABLE),yes)
  RUNNABLE_EXAMPLES += $(EXAMPLES_OBJC) $(EXAMPLES_OBJCC)
endif

run: all
	@status=0; \
	for bin in $(RUNNABLE_EXAMPLES); do \
		echo ""; \
		echo ">>> $$bin"; \
		$$bin || status=1; \
	done; \
	exit $$status

clean:
	rm -rf $(BUILD_DIR)

# --- Installation ---
PREFIX ?= /usr/local
INCLUDEDIR = $(PREFIX)/include
MANDIR = $(PREFIX)/share/man/man3

MAN_PAGES = man/cest.3 man/describe.3 man/it.3 man/test.3 \
            man/expect.3 man/expect_array.3 man/bench.3 \
            man/cest_init.3 man/cest_result.3

install: cest.h $(MAN_PAGES)
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	mkdir -p $(DESTDIR)$(MANDIR)
	cp cest.h $(DESTDIR)$(INCLUDEDIR)/cest.h
	for page in $(MAN_PAGES); do \
		cp $$page $(DESTDIR)$(MANDIR)/; \
	done
	chmod 644 $(DESTDIR)$(INCLUDEDIR)/cest.h
	chmod 644 $(DESTDIR)$(MANDIR)/*.3
	@echo "Cest installed to $(DESTDIR)$(PREFIX)"
	@if [ -z "$(DESTDIR)" ] && command -v mandb >/dev/null 2>&1; then \
		mandb -q; \
		echo "Manual database updated"; \
	fi

uninstall:
	rm -f $(DESTDIR)$(INCLUDEDIR)/cest.h
	for page in $(MAN_PAGES); do \
		rm -f $(DESTDIR)$(MANDIR)/$$(basename $$page); \
	done
	@echo "Cest removed from $(DESTDIR)$(PREFIX)"

test: run
