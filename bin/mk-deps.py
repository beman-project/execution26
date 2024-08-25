#!/usr/bin/python3

import re
import subprocess

deps = {}

done_re = re.compile('^\+ (?P<name>.*)')
with open("deps") as file:
    for line in file.readlines():
        match = done_re.match(line)
        if match:
            deps[match.group("name")] = 1

result = subprocess.run(["/usr/bin/tsort", "docs/dependency.txt"], capture_output=True, encoding="utf-8")
for line in result.stdout.split("\n"):
    print(f'{"+" if line in deps else "-"} {line}')