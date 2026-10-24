param([string]$Eboot="$PSScriptRoot/../EBOOT.PBP",[string]$Emulator="$PSScriptRoot/../../../work/ppsspp/PPSSPPWindows64.exe")
$ErrorActionPreference='Stop'
$shipRun=Join-Path (Split-Path $PSScriptRoot -Parent) ('outputs/ship-purchase-'+(Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
New-Item -ItemType Directory -Path $shipRun | Out-Null
Copy-Item -LiteralPath $Eboot -Destination (Join-Path $shipRun 'EBOOT.PBP')
Set-Content -LiteralPath (Join-Path $shipRun 'ship-purchase-review.flag') -Value '1'
$shipProcess=Start-Process -FilePath (Resolve-Path -LiteralPath $Emulator).Path -ArgumentList ('"'+(Join-Path $shipRun 'EBOOT.PBP')+'"') -WorkingDirectory $shipRun -WindowStyle Hidden -PassThru
try {
 $shipReport=Join-Path $shipRun 'ship-purchase-review.txt';$shipDeadline=(Get-Date).AddSeconds(60)
 while((Get-Date) -lt $shipDeadline){
  if((Test-Path -LiteralPath $shipReport)-and(Select-String -LiteralPath $shipReport -Pattern '^RESULT ' -Quiet)){break}
  Start-Sleep -Milliseconds 250
 }
 Get-Content -LiteralPath $shipReport
 Write-Output "Review: $shipRun"
 if(!(Select-String -LiteralPath $shipReport -Pattern '^RESULT 0 failures$' -Quiet)){throw 'Ship purchase checks failed or timed out'}
} finally {if(!$shipProcess.HasExited){Stop-Process -Id $shipProcess.Id}}
