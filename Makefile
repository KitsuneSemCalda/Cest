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
      OBJC_LIBS := -lobjc
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
  $(info To enable: install libobjc development package)
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

EXAMPLES = $(BUILD_DIR)/basic_c $(BUILD_DIR)/basic_cpp $(BUILD_DIR)/struct_testing $(BUILD_DIR)/multi_test_suite
ifeq ($(OBJC_AVAILABLE),yes)
  EXAMPLES += $(BUILD_DIR)/basic_objc $(BUILD_DIR)/basic_objcpp
endif

.PHONY: all run clean test objc-deps run-c run-cpp run-objc run-objcpp check-c11

all: $(BUILD_DIR) $(EXAMPLES)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/basic_c: examples/basic_c.c cest.h | check-c11
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/struct_testing: examples/struct_testing.c cest.h | check-c11
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/multi_test_suite: examples/multi_file/test_main.c examples/multi_file/test_math.c examples/multi_file/test_string.c cest.h | check-c11
	$(CC) $(CFLAGS) examples/multi_file/test_main.c examples/multi_file/test_math.c examples/multi_file/test_string.c -o $@

$(BUILD_DIR)/basic_cpp: examples/basic_cpp.cpp cest.h
	$(CXX) $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/basic_objc: examples/basic_objc.m cest.h
	$(OBJC_CC) $(OBJC_FLAGS) $< -o $@ $(OBJC_LIBS)

$(BUILD_DIR)/basic_objcpp: examples/basic_objcpp.mm cest.h
	$(OBJCC_CXX) $(OBJCC_FLAGS) $< -o $@ $(OBJC_LIBS) -L/usr/lib/gcc/x86_64-linux-gnu/$(GXX_MAJOR) -lstdc++

check-c11:
	@$(CC) $(CFLAGS) -std=c11 -x c -c -o /dev/null cest.h 2>/dev/null || \
		echo "Warning: C11 features may not be fully supported by your compiler"

run: all
	@echo "==========================================="
	@echo "Running Cest Examples"
	@echo "==========================================="
	@$(MAKE) run-c run-struct run-multi run-cpp
ifeq ($(OBJC_AVAILABLE),yes)
	@$(MAKE) run-objc run-objcpp
endif
	@echo "==========================================="

run-c:
	@echo ""
	@echo ">>> C Example"
	@echo "-------------------------------------------"
	@$(BUILD_DIR)/basic_c

run-struct:
	@echo ""
	@echo ">>> Struct Testing Example"
	@echo "-------------------------------------------"
	@$(BUILD_DIR)/struct_testing

run-multi:
	@echo ""
	@echo ">>> Multi-File Test Suite Example"
	@echo "-------------------------------------------"
	@$(BUILD_DIR)/multi_test_suite

run-cpp:
	@echo ""
	@echo ">>> C++ Example"
	@echo "-------------------------------------------"
	@$(BUILD_DIR)/basic_cpp

run-objc:
	@echo ""
	@echo ">>> Objective-C Example"
	@echo "-------------------------------------------"
	@$(BUILD_DIR)/basic_objc

run-objcpp:
	@echo ""
	@echo ">>> Objective-C++ Example"
	@echo "-------------------------------------------"
	@$(BUILD_DIR)/basic_objcpp

objc-deps:
	@echo "Checking Objective-C availability..."
	@if $(PKG_CONFIG) --exists objc 2>/dev/null; then \
		echo "pkg-config: objc found"; \
	elif $(PKG_CONFIG) --exists objc-gnu 2>/dev/null; then \
		echo "pkg-config: objc-gnu found"; \
	elif [ -f /usr/include/objc/objc.h ]; then \
		echo "libobjc headers found at /usr/include/objc/"; \
	else \
		echo "No Objective-C runtime detected."; \
		echo ""; \
		echo "Install libobjc-dev package:"; \
		echo "  Debian/Ubuntu: apt install libobjc-dev"; \
		echo "  Fedora/RHEL:   dnf install libobjc-devel"; \
		echo "  Arch:          pacman -S libobjc"; \
		echo "  macOS:         Included with Xcode/Developer Tools"; \
	fi

clean:
	rm -rf $(BUILD_DIR)

test: run
