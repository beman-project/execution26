# Copyright © 2024 Beman Project
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

.PHONY: default clean build test

default: test

build:
	@mkdir -p build
	cd build; cmake ..
	cmake --build build

test: build
	cd build; $(MAKE) test

clean:
	$(RM) mkerr older *~
	$(RM) -r build
