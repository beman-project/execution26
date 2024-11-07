#!/usr/bin/python3
# bin/mk-tody.py
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

import json
import re
import urllib.request

# -dk:TODO get from http://wg21.link/
url = "https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2024/p2300r10.html"

with urllib.request.urlopen(url) as f:
    html = f.read().decode("utf-8")

json = json.loads(open("docs/TODO.json").read())

list = re.findall('<a class="self-link" href="#spec-([^"]*)">', html)
list.append("allocator.requirements.general")
list.append("exec.awaitables")

fail = "&#x2705;"  # "&#9744;"
ok = "&#x1F534;"  # "&#9745;"

with open("docs/TODO.md", "w") as todo:
    print(f"# ToDo ", file=todo)
    print(f"", file=todo)
    print(f"| Section | Code | Test | Doc | Comment |", file=todo)
    print(f"| ------- |:----:|:----:|:---:| ------- |", file=todo)
    for name in list:
        node = json.get(name, {})
        if not node.get("removed", False):
            code = fail if node.get("code", False) else ok
            test = fail if node.get("test", False) else ok
            doc = fail if node.get("doc", False) else ok
            comment = node.get("comment", "")
            print(
                f"| [[{name}]({url}#{name})] | {code} | {test} | {doc} | {comment} |",
                file=todo,
            )
