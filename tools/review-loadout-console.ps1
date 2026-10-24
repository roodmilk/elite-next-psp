param([string]$Eboot="$PSScriptRoot/../EBOOT.PBP",[string]$Emulator="$PSScriptRoot/../../../work/ppsspp/PPSSPPWindows64.exe")
$ErrorActionPreference='Stop'
$consoleRun=Join-Path (Split-Path $PSScriptRoot -Parent) ('outputs/loadout-console-'+(Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
New-Item -ItemType Directory -Path $consoleRun | Out-Null
Copy-Item -LiteralPath $Eboot -Destination (Join-Path $consoleRun 'EBOOT.PBP')
Set-Content -LiteralPath (Join-Path $consoleRun 'loadout-console-review.flag') -Value '1'
$consoleProcess=Start-Process -FilePath (Resolve-Path -LiteralPath $Emulator).Path -ArgumentList ('"'+(Join-Path $consoleRun 'EBOOT.PBP')+'"') -WorkingDirectory $consoleRun -WindowStyle Hidden -PassThru
try {
 $consoleReport=Join-Path $consoleRun 'loadout-console-review.txt';$consoleDeadline=(Get-Date).AddSeconds(90)
 while((Get-Date) -lt $consoleDeadline){
  if((Test-Path -LiteralPath $consoleReport)-and(Select-String -LiteralPath $consoleReport -Pattern '^RESULT ' -Quiet)){break}
  Start-Sleep -Milliseconds 250
 }
 Get-Content -LiteralPath $consoleReport
 Write-Output "Review: $consoleRun"
 if(!(Select-String -LiteralPath $consoleReport -Pattern '^RESULT 0 failures$' -Quiet)){throw 'Loadout console checks failed or timed out'}
} finally {if(!$consoleProcess.HasExited){Stop-Process -Id $consoleProcess.Id}}
