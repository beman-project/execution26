# Makefile
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

MAKEFLAGS+= --no-builtin-rules          # Disable the built-in implicit rules.
MAKEFLAGS+= --warn-undefined-variables  # Warn when an undefined variable is referenced.

SANITIZERS = release debug usan # TODO: lsan
OS := $(shell /usr/bin/uname)
ifeq ($(OS),Darwin)
    SANITIZERS =+ tsan
endif
ifeq ($(OS),Linux)
    SANITIZERS =+ asan msan
endif

.PHONY: default doc run update check ce todo distclean clean codespell clang-tidy build test all format $(SANITIZERS)

SYSROOT   ?=
TOOLCHAIN ?=

COMPILER=c++
CXX_BASE=$(CXX:$(dir $(CXX))%=%)
ifeq ($(CXX_BASE),g++)
    COMPILER=g++
endif
ifeq ($(CXX_BASE),clang++)
    COMPILER=clang++
endif

CXX_FLAGS = -g
SANITIZER = release
SOURCEDIR = $(CURDIR)
BUILDROOT = build
BUILD     = $(BUILDROOT)/$(SANITIZER)
EXAMPLE   = beman.execution26.examples.stop_token
CMAKE_CXX_COMPILER=$(COMPILER)

ifeq ($(SANITIZER),release)
    CXX_FLAGS = -O3 -Wpedantic -Wall -Wextra -Wshadow # TODO: -Werror
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
	doxygen docs/Doxyfile

release: test

$(SANITIZERS):
	$(MAKE) SANITIZER=$@

build:
	@mkdir -p $(BUILD)
	cd $(BUILD); CC=$(CXX) cmake -G Ninja $(SOURCEDIR) $(TOOLCHAIN) $(SYSROOT) -DCMAKE_CXX_COMPILER=$(CXX) -DCMAKE_CXX_FLAGS="$(CXX_FLAGS) $(SAN_FLAGS)"
	cmake --build $(BUILD)

test: build
	# cmake --workflow --preset $(SANITIZER)
	ctest --test-dir $(BUILD) --rerun-failed --output-on-failure

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

build/$(SANITIZER)/compile_commands.json: $(SANITIZER)
clang-tidy: build/$(SANITIZER)/compile_commands.json
	run-clang-tidy -p build/$(SANITIZER) tests examples

codespell:
	codespell -L statics,snd,copyable,cancelled

format: cmake-format clang-format

cmake-format:
	cmake-format -i `git diff --name-only main | egrep '(CMakeLists.txt|\.cmake)'`

clang-format:
	git clang-format main

todo:
	bin/mk-todo.py

.PHONY: clean-doc
clean-doc:
	$(RM) -r docs/html docs/latex

clean: clean-doc
	$(RM) -r $(BUILD)
	$(RM) mkerr olderr *~

distclean: clean
	$(RM) -r $(BUILDROOT) stagedir
