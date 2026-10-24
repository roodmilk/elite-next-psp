param([string]$Eboot="$PSScriptRoot/../EBOOT.PBP",[string]$Emulator="$PSScriptRoot/../../../work/ppsspp/PPSSPPWindows64.exe")
$ErrorActionPreference='Stop'
$yardRun=Join-Path (Split-Path $PSScriptRoot -Parent) ('outputs/shipyard-console-'+(Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
New-Item -ItemType Directory -Path $yardRun | Out-Null
Copy-Item -LiteralPath $Eboot -Destination (Join-Path $yardRun 'EBOOT.PBP')
Set-Content -LiteralPath (Join-Path $yardRun 'shipyard-console-review.flag') -Value '1'
$yardProcess=Start-Process -FilePath (Resolve-Path -LiteralPath $Emulator).Path -ArgumentList ('"'+(Join-Path $yardRun 'EBOOT.PBP')+'"') -WorkingDirectory $yardRun -WindowStyle Hidden -PassThru
try {
 $yardReport=Join-Path $yardRun 'shipyard-console-review.txt';$yardDeadline=(Get-Date).AddSeconds(120)
 while((Get-Date) -lt $yardDeadline){
  if((Test-Path -LiteralPath $yardReport)-and(Select-String -LiteralPath $yardReport -Pattern '^RESULT ' -Quiet)){break}
  Start-Sleep -Milliseconds 250
 }
 Get-Content -LiteralPath $yardReport
 Write-Output "Review: $yardRun"
 if(!(Select-String -LiteralPath $yardReport -Pattern '^RESULT 0 failures$' -Quiet)){throw 'Shipyard console checks failed or timed out'}
}finally{if(!$yardProcess.HasExited){Stop-Process -Id $yardProcess.Id}}
