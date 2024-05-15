#!/usr/bin/python3
# Copyright © 2024 Beman Project
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

import json
import re
import urllib.request

#-dk:TODO get from http://wg21.link/
url = "https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2024/p2300r9.html"

with urllib.request.urlopen(url) as f:
    html = f.read().decode('utf-8')

json = json.loads(open("docs/TODO.json").read())

list = re.findall('<a class="self-link" href="#spec-([^"]*)">', html)

with open("docs/TODO.md", "w") as todo:
    print(f"# ToDo ", file=todo)
    print(f"", file=todo)
    print(f"| Section | Code | Test | Doc | Comment |", file=todo)
    print(f"| ------- |:----:|:----:|:---:| ------- |", file=todo)
    for name in list:
        node = json.get(name, {})
        if not node.get("removed", False):
            code = "&#9745;" if node.get("code", False) else "&#9744;"
            test = "&#9745;" if node.get("test", False) else "&#9744;"
            doc  = "&#9745;" if node.get("doc", False) else "&#9744;"
            comment = node.get("comment", "")
            print(f"| [[{name}]({url}#{name})] | {code} | {test} | {doc} | {comment} |", file=todo) 
