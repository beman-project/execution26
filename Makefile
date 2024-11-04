# Makefile
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

SANITIZERS = release debug msan asan usan tsan
.PHONY: default doc run update check ce todo distclean clean build test all format $(SANITIZERS)

COMPILER=system
CXX_BASE=$(CXX:$(dir $(CXX))%=%)
ifeq ($(CXX_BASE),g++)
    COMPILER=gcc
endif
ifeq ($(CXX_BASE),clang++)
    COMPILER=clang
endif

CXX_FLAGS = -g
SANITIZER = release
SOURCEDIR = $(shell pwd)
BUILDROOT = build
BUILD     = $(BUILDROOT)/$(SANITIZER)
EXAMPLE   = beman.execution26.examples.stop_token
CMAKE_C_COMPILER=$(COMPILER)
CMAKE_CXX_COMPILER=$(COMPILER)

ifeq ($(SANITIZER),release)
    CXX_FLAGS = -O3 -pedantic -Wall -Wextra -Werror
endif
ifeq ($(SANITIZER),debug)
    CXX_FLAGS = -g
endif
ifeq ($(SANITIZER),msan)
    SAN_FLAGS = -fsanitize=memory
endif
ifeq ($(SANITIZER),asan)
    SAN_FLAGS = -fsanitize=address -fsanitize=pointer-compare -fsanitize=pointer-subtract -fsanitize-address-use-after-scope
endif
ifeq ($(SANITIZER),usan)
    SAN_FLAGS = -fsanitize=undefined
endif
ifeq ($(SANITIZER),tsan)
    SAN_FLAGS = -fsanitize=thread
endif
ifeq ($(SANITIZER),lsan)
    SAN_FLAGS = -fsanitize=leak
endif

default: test

all: $(SANITIZERS)

run: test
	./$(BUILD)/examples/$(EXAMPLE)

doc:
	doxygen .doxy-config

release: test

$(SANITIZERS):
	$(MAKE) SANITIZER=$@

build:
	@mkdir -p $(BUILD)
	cd $(BUILD); CC=$(CXX) cmake $(SOURCEDIR) $(TOOLCHAIN) $(SYSROOT) -DCMAKE_CXX_COMPILER=$(CXX) -DCMAKE_CXX_FLAGS="$(CXX_FLAGS) $(SAN_FLAGS)"
	cmake --build $(BUILD)

test:
	cmake --workflow --preset $(SANITIZER)

ce:
	@mkdir -p $(BUILD)
	bin/mk-compiler-explorer.py $(BUILD)

SOURCE_CMAKELISTS = src/beman/execution26/CMakeLists.txt
update:
	bin/update-cmake-headers.py $(SOURCE_CMAKELISTS)

check:
	@for h in `find include -name \*.hpp`; \
	do \
		from=`echo -n $$h | sed -n 's@.*Beman/\(.*\).hpp.*@\1@p'`; \
		< $$h sed -n "/^ *# *include <Beman\//s@.*[</]Beman/\(.*\).hpp>.*@$$from \1@p"; \
	done | tsort > /dev/null

format:
	clang-format -i `git diff --name-only main | egrep '\.[ch]pp'`

todo:
	bin/mk-todo.py

clean:
	$(RM) -r $(BUILD)
	$(RM) mkerr olderr *~

distclean: clean
	$(RM) -r $(BUILDROOT) stagedir
