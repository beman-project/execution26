# Copyright © 2024 Beman Project
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

CONFIGS = none asan usan tsan
.PHONY: default clean build test all $(CONFIGS)

CXX_FLAGS =-g -pedantic -Wall -Wextra -Weffc++ -Werror
SANITIZER = 
SAN_FLAGS =
BUILDROOT = build
BUILD     = $(BUILDROOT)/none

ifeq ($(SANITIZER),asan)
    SAN_FLAGS = -fsanitize=address
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

default: test

all: $(CONFIGS)

none: test
asan:
	$(MAKE) SANITIZER=asan

usan:
	$(MAKE) SANITIZER=usan

tsan:
	$(MAKE) SANITIZER=tsan

build:
	@mkdir -p $(BUILD)
	cd $(BUILD); cmake ../.. -DCMAKE_CXX_FLAGS="$(CXX_FLAGS) $(SAN_FLAGS)"
	cmake --build $(BUILD)

test: build
	cd $(BUILD); $(MAKE) test

clean:
	$(RM) mkerr older *~
	$(RM) -r $(BUILDROOT)
