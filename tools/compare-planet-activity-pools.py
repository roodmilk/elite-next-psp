"""Compare pre/post PSP generation snapshots; allow only the empty-pool fallback."""
import sys
from pathlib import Path
from collections import defaultdict

def read(path):
    rows = {}
    for line in Path(path).read_text().splitlines():
        if line.startswith("SUMMARY"):
            continue
        fields = line.split()
        key = (int(fields[0]), int(fields[1]), int(fields[3]))
        if key in rows:
            raise ValueError(f"duplicate object: {key}")
        rows[key] = fields
    return rows

before, after = map(read, sys.argv[1:3])
assert before.keys() == after.keys(), "world/object identities changed"
worlds = defaultdict(list)
for key, fields in before.items():
    worlds[key[:2]].append(fields)
empty = {key for key, rows in worlds.items()
         if not any(row[4] == "2" and row[5] == "1" for row in rows)}
changed = 0
for key, fields in before.items():
    expected = fields[:]
    if key[:2] in empty and key[2] == 0:
        expected[4] = "2"
    assert after[key] == expected, f"unexpected semantic change: {key}"
    changed += fields != after[key]
assert changed == len(empty)
print(f"PASS {len(worlds)} worlds / {len(before)} objects; {changed} empty pools "
      "corrected by slot0 kind only; every position/other field unchanged")
