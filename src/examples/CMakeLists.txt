# Copyright © 2024 Beman Project
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

list(APPEND execution_examples
    stop_token
)
foreach(example ${execution_examples})
    add_executable(${example} ${example}.cpp)
endforeach()
