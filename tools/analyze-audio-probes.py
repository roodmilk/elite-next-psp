"""Compare silently decoded PSP music to reference PCM, including resampling."""
import pathlib
import sys
import numpy as np

root = pathlib.Path(sys.argv[1])
for track in ('spacebattle', 'spacebattle2', 'spacebattle3', 'spacebattle4'):
    refdir = root / (track + '-reference')
    report = dict(field.split('=') for field in (refdir / 'probe.txt').read_text().split())
    rate = int(report['rate'])
    ref = np.fromfile(refdir / 'decoded.pcm', dtype='<i2').astype(float).reshape(-1, 2)
    got = np.fromfile(root / (track + '-game') / 'decoded.pcm', dtype='<i2').astype(float).reshape(-1, 2)
    expected = np.column_stack([np.interp(np.arange(len(got)) * rate / 44100,
                               np.arange(len(ref)), ref[:, channel]) for channel in range(2)])
    correlation = np.corrcoef(expected.ravel(), got.ravel())[0, 1]
    clipping = np.mean(np.abs(got) >= 32760)
    assert len(got) == 44100 * 8 and correlation > .99 and clipping < .01
    print(f'PASS {track}: correlation={correlation:.6f} clipping={clipping:.6f}')
