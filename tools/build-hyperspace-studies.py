"""Recorded-source 48 kHz/24-bit design comparisons; never in-game evidence."""
import hashlib
import json
import wave
from pathlib import Path
import numpy as np
from importlib.util import spec_from_file_location, module_from_spec

ROOT = Path(__file__).resolve().parents[1]
spec = spec_from_file_location('flight_author', ROOT/'tools/build-flight-audio.py')
author = module_from_spec(spec)
spec.loader.exec_module(author)
RATE = 48000
OUT = ROOT/'audio/review/hyperspace-signatures'
OUT.mkdir(parents=True, exist_ok=True)
SOURCES = ROOT/'audio/sources/opengameart'

def source(name, cutoff):
    x, rate = author.read_wav(SOURCES/name)
    x = author.resample(x, rate, RATE)
    # Windowed-sinc low-pass removes abrasive upper harmonics before pitch changes.
    n = np.arange(255)-127
    kernel = 2*cutoff/RATE*np.sinc(2*cutoff/RATE*n)*np.hanning(255)
    x = np.convolve(x, kernel/kernel.sum(), mode='same')
    return author.normalise(x, .65)

def loop(x):
    overlap = min(4800, len(x)//4)
    blend = np.linspace(0, 1, overlap)
    return np.concatenate((x[overlap:-overlap], x[-overlap:]*(1-blend)+x[:overlap]*blend))

def movement(x, t, speed):
    pos = np.cumsum(speed)*RATE/RATE
    return np.interp(pos % len(x), np.arange(len(x)+1), np.r_[x,x[0]])

def envelope(t, points):
    return np.interp(t, [p[0] for p in points], [p[1] for p in points])

def delayed(x, seconds):
    count = int(seconds*RATE)
    return np.r_[np.zeros(count), x[:-count]]

def save(path, stereo):
    values = np.round(stereo.reshape(-1)*8388607).astype(np.int32)
    packed = np.column_stack((values&255, (values>>8)&255, (values>>16)&255)).astype(np.uint8)
    with wave.open(str(path),'wb') as w:
        w.setnchannels(2); w.setsampwidth(3); w.setframerate(RATE)
        w.writeframes(packed.tobytes())

engine = loop(source('racing-loop-0.wav', 1450))
fan = loop(source('fan-interval.wav', 700))
start = source('engine_start_up_01.wav', 1200)
metal = source('metal-thud3.wav', 900)
t = np.arange(RATE*10)/RATE
reports = {}
renders = []
for name, speed, width, tail, mechanical in [
    ('a-sheltered-pressure', .56, .28, .18, .12),
    ('b-open-corridor', .43, .68, .34, .06),
    ('c-heavy-drive', .36, .15, .12, .24),
]:
    charge = envelope(t,[(0,0),(.2,.13),(1,.2),(2.8,.5),(3.10,.62),(3.35,.025),(3.55,.025),(3.68,.85),(4,.56),(6.4,.40),(6.75,.57),(7.1,.24),(8.6,0),(10,0)])
    travel = envelope(t,[(0,0),(3.5,0),(3.8,.7),(6.4,1),(6.9,.2),(8,0),(10,0)])
    body = movement(engine,t,speed*(.8+.55*np.minimum(t/3.2,1)))*charge
    structure = movement(fan,t,np.full(len(t),speed*.68))*charge*mechanical
    # One actual engine-start movement, no repeated relay transients.
    engage = np.interp(t*RATE*.7,np.arange(len(start)),start,left=0,right=0)
    engage *= envelope(t,[(0,0),(.12,.12),(1.2,.08),(2,0),(10,0)])
    impact = np.zeros(len(t))
    for when, gain in [(3.58,.18),(6.75,.12)]:
        local = t-when
        hit = np.interp(local*RATE*.65,np.arange(len(metal)),metal,left=0,right=0)
        hit *= np.clip(local/.025,0,1)*np.clip((.9-local)/.16,0,1)
        impact += hit*gain
    centre = body+structure+engage+impact
    # Independent recorded machinery perspectives move around a centred drive.
    texture = movement(fan,t,np.full(len(t),speed*.91))*travel*.18
    pan = width*envelope(t,[(0,0),(3.6,-.8),(4.8,.8),(6.4,-.4),(7,0),(10,0)])
    left = centre+texture*(1-pan)+delayed(body,.041)*tail
    right = centre+texture*(1+pan)+delayed(body,.067)*tail
    stereo = np.column_stack((left,right))
    stereo *= envelope(t,[(0,0),(.08,1),(9,1),(10,0)])[:,None]
    stereo[0]=stereo[-1]=0
    renders.append((name,stereo))

# One common target RMS and shared peak bound preserve fair comparisons.
scales = [.085/np.sqrt(np.mean(x*x)) for _,x in renders]
peak = max(np.max(np.abs(x))*s for (_,x),s in zip(renders,scales))
ceiling = min(1,.60/peak)
montage=[]
for (name,x),scale in zip(renders,scales):
    x=x*scale*ceiling
    path=OUT/(name+'.wav'); save(path,x)
    reports[name]={'kind':'standalone recorded-source design study; not integrated',
        'rate':RATE,'bits':24,'seconds':10,'peak':float(np.max(np.abs(x))),
        'rms':float(np.sqrt(np.mean(x*x))), 'clipped_samples':int(np.count_nonzero(np.abs(x)>=1)),
        'largest_adjacent_step':float(np.max(np.abs(np.diff(x,axis=0)))),
        'quiet_contrast_db':float(20*np.log10(np.sqrt(np.mean(x[int(2.8*RATE):int(3.1*RATE)]**2))/np.sqrt(np.mean(x[int(3.35*RATE):int(3.55*RATE)]**2)))),
        'stereo_difference_rms':float(np.sqrt(np.mean((x[:,0]-x[:,1])**2))),
        'zero_endpoints':bool((x[[0,-1]]==0).all()),
        'sha256':hashlib.sha256(path.read_bytes()).hexdigest()}
    montage.extend((x,np.zeros((RATE,2))))
save(OUT/'comparison-a-b-c.wav',np.concatenate(montage))
(OUT/'render-report.json').write_text(json.dumps(reports,indent=2)+'\n')
print(json.dumps(reports,indent=2))
