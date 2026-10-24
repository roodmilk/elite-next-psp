param([string]$Eboot="$PSScriptRoot/../outputs/drive451-final-review/EBOOT64.PBP",[string]$Emulator="$PSScriptRoot/../../../work/ppsspp/PPSSPPWindows64.exe",[int]$Model=1)
$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent
$run=Join-Path $root ('outputs/flight-lighting-'+(Get-Date -Format 'yyyyMMdd-HHmmss'))
New-Item -ItemType Directory -Path $run | Out-Null
Copy-Item -LiteralPath $Eboot -Destination (Join-Path $run 'EBOOT.PBP')
Set-Content -LiteralPath (Join-Path $run 'flight-lighting-review.flag') -Value 1
$emulatorPath=(Resolve-Path -LiteralPath $Emulator).Path
$iniPath=Join-Path (Split-Path $emulatorPath -Parent) 'memstick/PSP/SYSTEM/ppsspp.ini'
$ini=[IO.File]::ReadAllText($iniPath)
$ini=$ini.Replace('IgnoreBadMemAccess = True','IgnoreBadMemAccess = False')
$ini=[regex]::Replace($ini,'GraphicsBackend = .*','GraphicsBackend = 2')
$ini=[regex]::Replace($ini,'PSPModel = \d',("PSPModel = $Model"))
$config=Join-Path $run 'isolated.ini'
[IO.File]::WriteAllText($config,$ini)
$process=Start-Process -FilePath $emulatorPath -ArgumentList @('--config="'+$config+'"','--log="'+(Join-Path $run 'emulator.log')+'"','"'+(Join-Path $run 'EBOOT.PBP')+'"') -WorkingDirectory $run -WindowStyle Hidden -PassThru
try {
 $deadline=(Get-Date).AddSeconds(45)
 while((Get-Date) -lt $deadline){
  if((Test-Path -LiteralPath (Join-Path $run 'flight-lighting-review.txt')) -and (Select-String -LiteralPath (Join-Path $run 'flight-lighting-review.txt') -Pattern '^RESULT ' -Quiet)){break}
  if($process.HasExited){throw 'PPSSPP exited before drive dynamics report'}
  Start-Sleep -Milliseconds 200
 }
 if(!(Test-Path -LiteralPath (Join-Path $run 'flight-lighting-review.txt'))){throw "Station lighting review timed out: $run"}
 Start-Sleep -Milliseconds 500
 Get-Content -LiteralPath (Join-Path $run 'flight-lighting-review.txt')
 Write-Output "Review: $run"
 if(!(Select-String -LiteralPath (Join-Path $run 'flight-lighting-review.txt') -Pattern '^RESULT 0 failures$' -Quiet)){throw "Station lighting/mapping test failed: $run"}
} finally {if(!$process.HasExited){Stop-Process -Id $process.Id}}



