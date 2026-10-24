"""Validate production silhouettes, unique solutions and one type per paper."""
from pathlib import Path
import re
from collections import Counter
ROOT=Path(__file__).resolve().parents[1]
source=(ROOT/'src/newspaper-picross-data.h').read_text()
pictures=[tuple(map(int,''.join(re.findall(r'"([01]{10})"',line))))
          for line in source.splitlines() if re.search(r'"[01]{10}"',line)]
def clue(a):
    return tuple(len(x) for x in ''.join(map(str,a)).split('0') if x) or (0,)
patterns=[tuple((x>>i)&1 for i in range(10)) for x in range(1024)]
def solutions(rows,cols):
    while True:
        old=sum(map(len,rows))+sum(map(len,cols))
        rows=[[a for a in rows[y] if all(any(b[y]==a[x] for b in cols[x]) for x in range(10))] for y in range(10)]
        cols=[[a for a in cols[x] if all(any(b[x]==a[y] for b in rows[y]) for y in range(10))] for x in range(10)]
        if any(not a for a in rows+cols):return 0
        if sum(map(len,rows))+sum(map(len,cols))==old:break
    if all(len(a)==1 for a in rows):return 1
    y=min((i for i in range(10) if len(rows[i])>1),key=lambda i:len(rows[i]))
    count=0
    for a in rows[y]:
        branch=list(rows);branch[y]=[a]
        count+=solutions(branch,cols)
        if count>1:return count
    return count
for i,p in enumerate(pictures):
    assert len(p)==100 and 0<sum(p)<100
    rows=[[a for a in patterns if clue(a)==clue(p[y*10:y*10+10])] for y in range(10)]
    cols=[[a for a in patterns if clue(a)==clue(p[x::10])] for x in range(10)]
    assert solutions(rows,cols)==1, f'Picture {i} is ambiguous'
    print(f'PASS picture {i}: exactly one solution')
s=(ROOT/'src/newspaper-data.h').read_text()
pages=[tuple(map(int,r)) for r in re.findall(r'\{(\d+),(\d+),(\d+),(\d+),(\d+)\}',s.split('newspaper_pages[]=',1)[1].split('};',1)[0])]
offset=list(map(int,re.search(r'newspaper_offsets\[256\]=\{([^}]+)',s)[1].split(',')))
end=int(re.search(r'newspaper_earth_offset=(\d+)',s)[1])
c=(ROOT/'src/newspaper-composition-data.h').read_text()
overrides={(int(a),int(b)):int(kind) for a,b,kind in re.findall(r'\{(\d+),(\d+),\d+,\d+,(\d+),"',c)}
authored={int(i) for i in re.findall(r'\{(\d+),\d+,\{[^}]+\},"',c)}
counts=Counter();seen=set();examples=[]
for system in range(256):
    kinds=set()
    for page in range(offset[system],offset[system+1] if system<255 else end):
        kind=overrides.get((system,page-offset[system]),pages[page][3])
        if kind and system not in authored and system%3==0:kind=3
        if kind:kinds.add(kind)
    assert len(kinds)==1,(system,kinds)
    kind=kinds.pop();counts[kind]+=1
    if kind==3:seen.add((system//3)%6);examples.append(system)
assert seen==set(range(6))
print('PASS all 256 newspapers expose exactly one puzzle type')
print('Assigned papers:',dict(counts),'(1 Sudoku, 2 crossword, 3 Picross)')
print('Picross systems:',examples)
print('RESULT 0 failures')
