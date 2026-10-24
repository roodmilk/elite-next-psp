from pathlib import Path
from PIL import Image
import hashlib,json,csv
root=Path(__file__).resolve().parents[1];source=root.parent/'erl-implementation-candidate/outputs/magnetic-slice/regression-nav486-i-footage-diagnostic-encounter-flight-capture';out=root/'assets/generated/navigation486';out.mkdir(parents=True,exist_ok=True)
binding=json.loads((source/'binding.json').read_text(encoding='utf-8-sig'));assert binding['footage'] and binding['model']==1 and binding['internal_resolution']==1
rows=list(csv.reader((source/'twin-footage.csv').open()));assert len(rows)==80
frames=[];durations=[];hashes=[]
for row in rows:
 frame=int(row[0]);path=source/f'twin-frame-{frame:03d}.rgba';raw=path.read_bytes();assert len(raw)==512*272*4
 image=Image.frombytes('RGBA',(512,272),raw).crop((0,0,480,272)).convert('RGB');frames.append(image);durations.append(max(1,round(float(row[1])*1000)));hashes.append(hashlib.sha256(image.tobytes()).hexdigest())
frames[0].save(out/'retail-cloud-motion.apng',save_all=True,append_images=frames[1:],duration=durations,loop=0,disposal=0,blend=0)
frames[40].save(out/'retail-cloud-native.png')
with Image.open(out/'retail-cloud-motion.apng') as animation:
 assert animation.n_frames==80
 for i in range(80):animation.seek(i);assert hashlib.sha256(animation.convert('RGB').tobytes()).hexdigest()==hashes[i]
proof={'eboot_sha256':binding['eboot_sha256'],'dimensions':[480,272],'frames':80,'recorded_duration_ms':sum(durations),'timing':'Recorded frame intervals. Capture file I/O overhead excluded from separate FPS tests. Scripted orbit in real retail main loop, mixer and display.','pixel_identical_to_native_rgb':True,'frame_rgb_sha256':hashes,'source_csv':str(source/'twin-footage.csv')}
(out/'retail-footage-proof.json').write_text(json.dumps(proof,indent=2))
page=out/'index.html';s=page.read_text();section='<figure><figcaption>Actual retail game capture — 80 native frames, recorded timing; scripted camera. Recording overhead is excluded from FPS measurements.</figcaption><img width="480" height="272" src="retail-cloud-motion.apng"></figure><p><a href="retail-footage-proof.json">Lossless native-frame verification</a></p>'
s=s.replace('<p>The reference is',section+'<p>The reference is',1);page.write_text(s)
print(json.dumps({k:v for k,v in proof.items() if k!='frame_rgb_sha256'},indent=2))
