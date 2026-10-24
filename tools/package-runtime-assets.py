"""Append a checked seekable print pack without changing any intro-film byte."""
from pathlib import Path
import argparse,shutil,struct

def fnv(data):
    h=2166136261
    for byte in data:h=((h^byte)*16777619)&0xffffffff
    return h

def footer(intro_length,pack):
    head=struct.pack('<8s5I',b'ENASSET1',1,intro_length,len(pack),fnv(pack),0)
    return head+struct.pack('<I',fnv(head))

def bounds(data):
    if len(data)<32 or data[-32:-24]!=b'ENASSET1':return None
    magic,version,intro_size,pack_size,pack_hash,reserved,check=struct.unpack('<8s6I',data[-32:])
    assert version==1 and reserved==0 and fnv(data[-32:-4])==check
    assert intro_size>=40 and pack_size>=24 and intro_size+pack_size+32==len(data)
    assert fnv(data[intro_size:-32])==pack_hash
    return intro_size,pack_size

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--intro',type=Path,required=True);ap.add_argument('--pack',type=Path,required=True);ap.add_argument('--output',type=Path,required=True);args=ap.parse_args()
    pack=args.pack.read_bytes();size=args.intro.stat().st_size
    args.output.parent.mkdir(parents=True,exist_ok=True)
    with args.output.open('wb') as out,args.intro.open('rb') as intro:
        shutil.copyfileobj(intro,out,1024*1024);out.write(pack);out.write(footer(size,pack))
    print(f'PSAR: {size} unchanged intro bytes + {len(pack)} print bytes + 32-byte checked footer')

if __name__=='__main__':main()
