param([string]$Eboot="$PSScriptRoot/../outputs/audio-dynamic-flight/EBOOT.PBP",[string]$Emulator="$PSScriptRoot/../../../work/ppsspp/PPSSPPWindows64.exe",[int]$Model=1,[string]$Music='', [ValidateSet(1,2,3,4)][int]$Mode=1,[switch]$CaptureAudio)
$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent
$run=Join-Path $root ('outputs/audio-flight-capture-'+(Get-Date -Format 'yyyyMMdd-HHmmss'))
New-Item -ItemType Directory -Path $run | Out-Null
Copy-Item -LiteralPath $Eboot -Destination (Join-Path $run 'EBOOT.PBP')
Set-Content -LiteralPath (Join-Path $run 'audio-flight-capture.flag') -Value 1
if($Mode -gt 1){Set-Content -LiteralPath (Join-Path $run 'audio-music-stress.flag') -Value $Mode}
if($Music){
 $folder=Join-Path $run $(if($Mode -eq 3){'music/Thargoid Battle'}else{'music/Elite Exploration'})
 New-Item -ItemType Directory -Force $folder | Out-Null
 Copy-Item -LiteralPath $Music -Destination (Join-Path $folder ('test'+[IO.Path]::GetExtension($Music)))
}
$emulatorPath=(Resolve-Path -LiteralPath $Emulator).Path
$iniPath=Join-Path (Split-Path $emulatorPath -Parent) 'memstick/PSP/SYSTEM/ppsspp.ini'
$ini=[IO.File]::ReadAllText($iniPath)
$ini=$ini.Replace('IgnoreBadMemAccess = True','IgnoreBadMemAccess = False')
$ini=[regex]::Replace($ini,'GraphicsBackend = .*','GraphicsBackend = 2')
$ini=[regex]::Replace($ini,'PSPModel = \d',("PSPModel = $Model"))
# Host speaker buffering is independent of the game's PSP PCM queue.
# Tiny 256-frame host buffers crackle under desktop capture/render load.
$ini=$ini.Replace('ExtraAudioBuffering = False','ExtraAudioBuffering = True')
$ini=[regex]::Replace($ini,'AudioBufferSize = \d+','AudioBufferSize = 1024')
if($CaptureAudio){$ini=$ini.Replace('DumpAudio = False','DumpAudio = True')}
$config=Join-Path $run 'isolated.ini' 
[IO.File]::WriteAllText($config,$ini)
$process=Start-Process -FilePath $emulatorPath -ArgumentList @('--config="'+$config+'"','--log="'+(Join-Path $run 'emulator.log')+'"','"'+(Join-Path $run 'EBOOT.PBP')+'"') -WorkingDirectory $run -WindowStyle Hidden -PassThru
try {
 $deadline=(Get-Date).AddSeconds(70)
 while((Get-Date) -lt $deadline){
  if((Test-Path -LiteralPath (Join-Path $run 'audio-flight-performance.txt')) -and (Select-String -LiteralPath (Join-Path $run 'audio-flight-performance.txt') -Pattern '^RESULT ' -Quiet)){break}
  if($process.HasExited){throw 'PPSSPP exited before flight-audio report'}
  Start-Sleep -Milliseconds 250
 }
 if(!(Test-Path -LiteralPath (Join-Path $run 'audio-flight-performance.txt'))){throw "Flight audio review timed out: $run"}
 Start-Sleep -Milliseconds 750
 Get-Content -LiteralPath (Join-Path $run 'audio-flight-performance.txt')
 Write-Output "Review: $run"
 if(!(Select-String -LiteralPath (Join-Path $run 'audio-flight-performance.txt') -Pattern '^RESULT 0 failures$' -Quiet)){throw "Flight/audio test failed: $run"}
} finally {if(!$process.HasExited){Stop-Process -Id $process.Id}}
