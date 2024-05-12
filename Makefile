# Copyright © 2024 Beman Project
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

SANITIZERS = none asan usan tsan
.PHONY: default clean build test all $(SANITIZERS)

CXX_FLAGS = -O3
SANITIZER = none
BUILDROOT = build
BUILD     = $(BUILDROOT)/none

ifeq ($(SANITIZER),none)
    CXX_FLAGS = -O3 -pedantic -Wall -Wextra -Werror
endif
ifeq ($(SANITIZER),asan)
    SAN_FLAGS = -fsanitize=address -fsanitize=pointer-compare -fsanitize=pointer-subtract -fsanitize-address-use-after-scope

    BUILD     = $(BUILDROOT)/asan
endif
ifeq ($(SANITIZER),usan)
    SAN_FLAGS = -fsanitize=undefined
    BUILD     = $(BUILDROOT)/usan
endif
ifeq ($(SANITIZER),tsan)
    SAN_FLAGS = -fsanitize=thread
    BUILD     = $(BUILDROOT)/tsan
endif
ifeq ($(SANITIZER),lsan)
    SAN_FLAGS = -fsanitize=leak
    BUILD     = $(BUILDROOT)/lsan
endif

default: test

all: $(SANITIZERS)

none: test
asan:
	$(MAKE) SANITIZER=asan

usan:
	$(MAKE) SANITIZER=usan

tsan:
	$(MAKE) SANITIZER=tsan

lsan:
	$(MAKE) SANITIZER=lsan

build:
	@mkdir -p $(BUILD)
	cd $(BUILD); CC=$(CXX) cmake ../.. -DCMAKE_CXX_COMPILER=$(CXX) -DCMAKE_CXX_FLAGS="$(CXX_FLAGS) $(SAN_FLAGS)"
	cmake --build $(BUILD)

test: build
	cd $(BUILD); $(MAKE) test

clean:
	$(RM) mkerr older *~
	$(RM) -r $(BUILDROOT)
