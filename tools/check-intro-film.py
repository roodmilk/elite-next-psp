"""Validate the actual native movie archive or its embedded EBOOT.PBP payload."""
from pathlib import Path
from PIL import Image
import hashlib, io, json, struct, sys

p=Path(sys.argv[1])
data=p.read_bytes()
base=struct.unpack_from('<I',data,36)[0] if data[:4]==b'\0PBP' else 0
assert data[base:base+8]==b'ENINTRO1', 'Missing embedded intro'
version,width,height,fps,count,loop,maximum,checksum=struct.unpack_from('<8I',data,base+8)
assert (version,width,height,fps)==(1,480,272,30)
assert count==1440 and loop==1200 and maximum<=131072
table=data[base+40:base+40+(count+1)*4]
actual=2166136261
for b in table: actual=((actual^b)*16777619)&0xffffffff
assert actual==checksum, 'Index checksum'
offsets=struct.unpack('<'+str(count+1)+'I',table)
assert offsets[0]==40+len(table) and offsets[-1]==len(data)-base
for i,(a,b) in enumerate(zip(offsets,offsets[1:])):
    assert 4<=b-a<=maximum, i
    frame=data[base+a:base+b]
    assert frame[:2]==b'\xff\xd8' and frame[-2:]==b'\xff\xd9', i
    im=Image.open(io.BytesIO(frame))
    assert im.size==(480,272) and im.format=='JPEG', i
    assert not im.info.get('progressive'), i
    assert im.layer==[(1,2,2,0),(2,1,1,1),(3,1,1,1)], ('Not PSP-compatible 4:2:0',i,im.layer)
    if i%120==0 or i==count-1: im.load()
print(json.dumps({'path':str(p),'bytes':len(data),'sha256':hashlib.sha256(data).hexdigest().upper(),'native_frames':count,'fps':fps,'loop_start_seconds':loop/fps,'max_jpeg_bytes':maximum,'all_frame_headers':'PASS','sampled_full_decodes':'PASS','psp_yuv420_sampling':'PASS'},indent=2))
