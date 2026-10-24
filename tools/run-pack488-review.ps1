param(
 [ValidateSet('cockpit-hud-review','final-menu-review')][string]$Flag='cockpit-hud-review',
 [string]$Eboot="$PSScriptRoot/../build-pack488-review/EBOOT.PBP"
)
$ErrorActionPreference='Stop'
$root=(Resolve-Path "$PSScriptRoot/..").Path
$candidate=(Resolve-Path "$PSScriptRoot/../../erl-implementation-candidate").Path
$run=Join-Path $root "outputs/pack488-$Flag"
$private=Join-Path $run 'emulator'
$system=Join-Path $private 'memstick/PSP/SYSTEM'
New-Item -ItemType Directory -Force $system | Out-Null
Copy-Item (Resolve-Path $Eboot).Path (Join-Path $run 'EBOOT.PBP') -Force
Copy-Item (Join-Path $candidate 'emulator/PPSSPPWindows64.exe') $private -Force
Copy-Item (Join-Path $candidate 'emulator/assets') $private -Recurse -Force
Get-ChildItem (Join-Path $candidate 'emulator') -Filter '*.dll' | Copy-Item -Destination $private -Force
Copy-Item (Join-Path $candidate 'emulator/memstick/PSP/SYSTEM/controls.ini') $system -Force
$ini=[IO.File]::ReadAllText((Join-Path $candidate 'evidence/baseline/isolated.ini'))
$ini=[regex]::Replace($ini,'PSPModel = \d','PSPModel = 1')
$ini=[regex]::Replace($ini,'(?m)^(GameVolume|UIVolume|AchievementVolume|GamePreviewVolume)\s*=.*$','$1 = 0')
$ini=[regex]::Replace($ini,'(?m)^UISound\s*=.*$','UISound = False')
$config=Join-Path $run 'isolated.ini'
[IO.File]::WriteAllText($config,$ini,[Text.UTF8Encoding]::new($false))
$stem=if($Flag -eq 'final-menu-review'){'final-menu'}else{$Flag}
Set-Content (Join-Path $run "$stem.flag") 1
$report=Join-Path $run "$stem.txt"
Remove-Item $report -Force -ErrorAction SilentlyContinue
$process=Start-Process (Join-Path $private 'PPSSPPWindows64.exe') -ArgumentList @('--config='+$config,'--controlconfig='+(Join-Path $system 'controls.ini'),'--log='+(Join-Path $run 'emulator.log'),(Join-Path $run 'EBOOT.PBP')) -WorkingDirectory $run -WindowStyle Hidden -PassThru
try {
 $deadline=(Get-Date).AddSeconds(60)
 while((Get-Date)-lt $deadline){
  if((Test-Path $report)-and(Select-String -LiteralPath $report -Pattern '^RESULT ' -Quiet)){break}
  if($process.HasExited){break}
  Start-Sleep -Milliseconds 250
 }
 if(!(Test-Path $report)){throw "Review report missing: $report"}
 Get-Content $report
 if(!(Select-String -LiteralPath $report -Pattern '^RESULT 0 failures' -Quiet)){throw "$Flag failed"}
} finally {if(!$process.HasExited){Stop-Process $process.Id}}
