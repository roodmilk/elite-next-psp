param([string]$OutputRoot,[string]$TrackPath='C:\Users\skarm\Downloads\spacebattle.ogg')
$ErrorActionPreference='Stop'
$probeRoot=Split-Path $PSScriptRoot -Parent
$probeToolchain=(Resolve-Path "$probeRoot/../../work/toolchain").Path
$probeSdk="$probeToolchain/psp/sdk"
$env:PATH="$probeToolchain/bin;"+$env:PATH
foreach($mode in @('tremor','reference','game')){
 $dir=Join-Path $OutputRoot $mode
 New-Item -ItemType Directory -Force -Path $dir | Out-Null
 $defs=@();$libs=@('-lvorbisfile','-lvorbis','-logg')
 if($mode -eq 'game'){$defs=@('-DPROBE_GAME')}
 if($mode -eq 'tremor'){$defs=@('-DPROBE_TREMOR');$libs=@('-lvorbisidec','-logg')}
 & "$probeToolchain/bin/psp-gcc.exe" -O2 -G0 "-I$probeSdk/include" @defs -c "$PSScriptRoot/audio-decoder-probe.c" -o "$dir/probe.o"
 if($LASTEXITCODE){throw 'Probe compile failed'}
 & "$probeToolchain/bin/psp-gcc.exe" -G0 "-L$probeSdk/lib" "-specs=$probeSdk/lib/prxspecs" "-Wl,-q,-T$probeSdk/lib/linkfile.prx" '-Wl,-zmax-page-size=128' "$dir/probe.o" "$probeSdk/lib/prxexports.o" @libs -lpspaudio -lpspmp3 -lpsputility -lm -o "$dir/probe.elf"
 if($LASTEXITCODE){throw 'Probe link failed'}
 & "$probeToolchain/bin/psp-fixup-imports.exe" "$dir/probe.elf"
 & "$probeToolchain/bin/psp-prxgen.exe" "$dir/probe.elf" "$dir/probe.prx"
 & "$probeToolchain/bin/mksfoex.exe" -d MEMSIZE=0 SilentAudioProbe "$dir/PARAM.SFO"
 & "$probeToolchain/bin/pack-pbp.exe" "$dir/EBOOT.PBP" "$dir/PARAM.SFO" NULL NULL NULL NULL NULL "$dir/probe.prx" NULL
 Copy-Item -LiteralPath $TrackPath -Destination "$dir/input.ogg"
}
