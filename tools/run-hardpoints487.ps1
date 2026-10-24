param([string]$Flag='hardpoint-review',[string]$Tag='hardpoints487')
$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent
$base=Join-Path $root '../erl-implementation-candidate'
$run=Join-Path $root "outputs/$Tag-$Flag"
$private=Join-Path $run 'emulator'
New-Item -ItemType Directory -Force "$private/memstick/PSP/SYSTEM" | Out-Null
Copy-Item "$base/emulator/PPSSPPWindows64.exe" $private
Copy-Item "$base/emulator/assets" $private -Recurse -Force
Get-ChildItem "$base/emulator" -Filter '*.dll' | ForEach-Object {Copy-Item $_.FullName $private -Force}
Copy-Item "$root/build-hardpoints487-review/EBOOT.PBP" $run -Force
$ini=[IO.File]::ReadAllText("$base/evidence/baseline/isolated.ini")
$ini=[regex]::Replace($ini,'PSPModel = \d','PSPModel = 1')
$ini=[regex]::Replace($ini,'(?m)^(GameVolume|UIVolume|AchievementVolume|GamePreviewVolume)\s*=.*$','$1 = 0')
$ini=[regex]::Replace($ini,'(?m)^(DumpFrames|DumpVideoOutput|DumpAudio)\s*=.*$','$1 = False')
[IO.File]::WriteAllText("$run/isolated.ini",$ini)
Set-Content "$run/$Flag.flag" 1
$process=Start-Process "$private/PPSSPPWindows64.exe" -ArgumentList @('--config="'+"$run/isolated.ini"+'"','"'+"$run/EBOOT.PBP"+'"') -WorkingDirectory $run -WindowStyle Hidden -PassThru
try{$until=(Get-Date).AddSeconds(50);while((Get-Date)-lt$until){if(Test-Path "$run/$Flag.txt"){if(Select-String -LiteralPath "$run/$Flag.txt" -Pattern '^RESULT ' -Quiet){break}};if($process.HasExited){break};Start-Sleep -Milliseconds 250};Get-Content "$run/$Flag.txt"}finally{if(!$process.HasExited){Stop-Process -Id $process.Id}}

