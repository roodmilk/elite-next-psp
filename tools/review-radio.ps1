param([string]$Eboot="$PSScriptRoot/../EBOOT.PBP",[string]$Emulator="$PSScriptRoot/../../../work/ppsspp/PPSSPPWindows64.exe",[int]$Model=1)
$ErrorActionPreference='Stop'
$radioRun=Join-Path (Split-Path $PSScriptRoot -Parent) ('outputs/radio-review-'+(Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
New-Item -ItemType Directory -Path $radioRun | Out-Null
Copy-Item -LiteralPath $Eboot -Destination (Join-Path $radioRun 'EBOOT.PBP')
Set-Content -LiteralPath (Join-Path $radioRun 'radio-check.flag') -Value 1
$radioEmulator=(Resolve-Path -LiteralPath $Emulator).Path
$radioIni=[IO.File]::ReadAllText((Join-Path (Split-Path $radioEmulator -Parent) 'memstick/PSP/SYSTEM/ppsspp.ini'))
$radioIni=$radioIni.Replace('IgnoreBadMemAccess = True','IgnoreBadMemAccess = False')
$radioIni=[regex]::Replace($radioIni,'GraphicsBackend = .*','GraphicsBackend = 2')
$radioIni=[regex]::Replace($radioIni,'PSPModel = \d',("PSPModel = $Model"))
$radioConfig=Join-Path $radioRun 'isolated.ini';[IO.File]::WriteAllText($radioConfig,$radioIni)
$radioProcess=Start-Process -FilePath $radioEmulator -ArgumentList @('--config="'+$radioConfig+'"','"'+(Join-Path $radioRun 'EBOOT.PBP')+'"') -WorkingDirectory $radioRun -WindowStyle Hidden -PassThru
try {
 $radioReport=Join-Path $radioRun 'radio-check.txt';$radioDeadline=(Get-Date).AddSeconds(180)
 while((Get-Date) -lt $radioDeadline){if((Test-Path -LiteralPath $radioReport)-and(Select-String -LiteralPath $radioReport -Pattern '^RESULT ' -Quiet)){break};Start-Sleep -Milliseconds 250}
 Get-Content -LiteralPath $radioReport
 Write-Output "Review: $radioRun"
 if(!(Select-String -LiteralPath $radioReport -Pattern '^RESULT 0 failures$' -Quiet)){throw 'Radio checks failed or timed out'}
} finally {if(!$radioProcess.HasExited){Stop-Process -Id $radioProcess.Id}}
