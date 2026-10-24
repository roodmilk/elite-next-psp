"""Compile bounded per-edition print records and native art into a seekable pack.

The pack is designed for EBOOT's DATA.PSAR, following the unchanged intro film.
Only one edition and two native illustrations need to be resident at a time.
This tool does not create prose, regenerate drafts or mark pages inspected.
"""
from pathlib import Path
import argparse,hashlib,importlib.util,json,struct
from PIL import Image

ROOT=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('print_compositions',ROOT/'tools/compile-newspaper-compositions.py')
composition=importlib.util.module_from_spec(spec);spec.loader.exec_module(composition)
MAX_ISSUE=131072
MAX_ART_PIXELS=24576

def fnv(data):
    h=2166136261
    for byte in data:h=((h^byte)*16777619)&0xffffffff
    return h

def fixed(text,size):
    raw=text.encode('ascii');assert len(raw)<size
    return raw+b'\0'*(size-len(raw))

def issue_record(issue,collection,marks,asset_names):
    pool=bytearray(b'\0');intern={'':0}
    def string(text):
        if text not in intern:
            raw=text.encode('ascii');assert len(raw)<=255 and b'\0' not in raw
            intern[text]=len(pool);pool.extend(raw+b'\0')
        return intern[text]
    pages=[];ops=[]
    for page in issue['pages']:
        first=len(ops)
        for op in composition.expanded(page):
            kind=op['kind'];x,y=op['x'],op['y'];w,h=op.get('w',0),op.get('h',0);arg=0;word=''
            if kind=='text':
                word=op['text'];arg=op.get('scale',1);assert arg in (1,2)
                w,h=len(word)*8*arg,8*arg
            elif kind=='art':
                arg=asset_names.index(op['asset'])
            assert 0<=x and 84<=y and x+w<=480 and y+h<=248
            ops.append(struct.pack('<hhhhIHHI',x,y,w,h,composition.colour(op.get('ink',issue['palette'][1])),
                {'box':0,'text':1,'art':2,'puzzle':3}[kind],arg,string(word)))
        puzzle=collection[issue['system']]['pages'][page['page']].get('puzzle',0) if any(o['kind']=='puzzle' for o in page['ops']) else 0
        pages.append(struct.pack('<HHHHI',page['page'],first,len(ops)-first,puzzle,string(page['recipe'])))
    assert len(pages)==collection[issue['system']]['identity']['page_count']<=24
    assert len(ops)<=4096
    palette=[composition.colour(c) for c in issue['palette']]
    header=struct.pack('<HH6I32s48s16sI',issue['system'],len(pages),*palette,
        fixed(issue['masthead'],32),fixed(issue['motto'],48),fixed(issue['name'],16),len(ops))
    assert len(header)==128
    mark=bytes('.#+o*'.index(ch) for row in marks[issue['mark']] for ch in row);assert len(mark)==256
    record=header+mark+b''.join(pages)+b''.join(ops)+pool
    record+=b'\0'*(-len(record)%4)
    assert len(record)<=MAX_ISSUE
    return record

def compile_pack():
    data=json.loads((ROOT/'content/newspapers/approved-issues.json').read_text())
    collection=json.loads((ROOT/'content/newspapers/collection.json').read_text())
    marks=json.loads((ROOT/'content/newspapers/print-marks.json').read_text())
    assets=composition.ASSETS;asset_names=list(assets)
    payload=[];issue_index=[];art_index=[]
    offset=24+16*len(data['issues'])+20*len(assets)
    for issue in sorted(data['issues'],key=lambda i:i['system']):
        blob=issue_record(issue,collection,marks,asset_names)
        issue_index.append(struct.pack('<HHIII',issue['system'],len(issue['pages']),offset,len(blob),fnv(blob)))
        payload.append(blob);offset+=len(blob)
    for name,path in assets.items():
        im=Image.open(ROOT/'assets/source/newspaper-approved'/path).convert('RGB')
        assert im.width<=256 and im.height<=160 and im.width*im.height<=MAX_ART_PIXELS
        pixels=[((r>>3)<<11)|((g>>2)<<5)|(b>>3) for r,g,b in im.getdata()]
        blob=struct.pack('<%dH'%len(pixels),*pixels)
        art_index.append(struct.pack('<HHIIII',im.width,im.height,offset,len(blob),fnv(blob),0))
        payload.append(blob);offset+=len(blob)
    directory=b''.join(issue_index+art_index)
    header=struct.pack('<8s4I',b'NPACK001',1,len(issue_index),len(art_index),fnv(directory))
    result=header+directory+b''.join(payload)
    assert len(result)==offset
    return result,dict(schema=1,issues=len(issue_index),pages=sum(len(i['pages']) for i in data['issues']),art=len(art_index),
        length=len(result),fnv=fnv(result),sha256=hashlib.sha256(result).hexdigest(),
        maximum_edition_bytes=max(len(b) for b in payload[:len(issue_index)]),
        maximum_native_art_bytes=max(len(b) for b in payload[len(issue_index):]),
        runtime_memory_policy='one edition <=128 KiB; two native art buffers <=48 KiB each')

def inspect_pack(blob):
    """Strict reference reader used to exercise corrupt and truncated records."""
    assert len(blob)>=24
    magic,version,ni,na,checksum=struct.unpack_from('<8s4I',blob)
    assert magic==b'NPACK001' and version==1 and 1<=ni<=257 and 1<=na<=512
    end=24+16*ni+20*na;assert end<=len(blob) and fnv(blob[24:end])==checksum
    systems=set();pages_seen=0
    for n in range(ni):
        system,np,offset,length,check=struct.unpack_from('<HHIII',blob,24+n*16)
        assert system<=256 and system not in systems and 4<=np<=24 and end<=offset<=len(blob)-length and 384<=length<=MAX_ISSUE
        systems.add(system);record=blob[offset:offset+length];assert fnv(record)==check
        sys2,np2,*identity=struct.unpack_from('<HH6I32s48s16sI',record)
        assert (sys2,np2)==(system,np)
        op_count=identity[-1];assert op_count<=4096
        assert all(v<=4 for v in record[128:384])
        page_end=384+np*12;pool=page_end+op_count*20;assert pool<len(record)
        def string(at):
            assert 0<=at<len(record)-pool
            stop=record.find(b'\0',pool+at);assert pool+at<=stop<=pool+at+255
            value=record[pool+at:stop];assert all(32<=b<=126 for b in value)
            return value
        seen_pages=set()
        for i in range(np):
            page,first,count,puzzle,recipe=struct.unpack_from('<HHHHI',record,384+i*12)
            assert page<np and page not in seen_pages and first+count<=op_count and puzzle<=2
            seen_pages.add(page);string(recipe)
        for i in range(op_count):
            x,y,w,h,ink,kind,arg,text=struct.unpack_from('<hhhhIHHI',record,page_end+i*20)
            assert 0<=x and 84<=y and w>=0 and h>=0 and x+w<=480 and y+h<=248 and kind<=3
            word=string(text)
            if kind==1:assert arg in (1,2) and (w,h)==(len(word)*8*arg,8*arg)
            if kind==2:assert arg<na
        pages_seen+=np
    for i in range(na):
        w,h,offset,length,check,format=struct.unpack_from('<HHIIII',blob,24+ni*16+i*20)
        assert 1<=w<=256 and 1<=h<=160 and w*h<=MAX_ART_PIXELS and length==w*h*2 and format==0
        assert end<=offset<=len(blob)-length and fnv(blob[offset:offset+length])==check
    return pages_seen

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--output',type=Path,default=ROOT/'assets/newspapers/print.npk');args=ap.parse_args()
    blob,manifest=compile_pack();assert inspect_pack(blob)==manifest['pages']
    args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_bytes(blob)
    args.output.with_suffix('.json').write_text(json.dumps(manifest,indent=2)+'\n',encoding='utf-8')
    print(json.dumps(manifest,indent=2))

if __name__=='__main__':main()
