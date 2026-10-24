param([string]$Eboot="$PSScriptRoot/../EBOOT.PBP",[string]$Emulator="$PSScriptRoot/../../../work/ppsspp/PPSSPPWindows64.exe")
$ErrorActionPreference='Stop'
$commsRun=Join-Path (Split-Path $PSScriptRoot -Parent) ('outputs/comms-panel-review-'+(Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
New-Item -ItemType Directory -Path $commsRun | Out-Null
Copy-Item -LiteralPath $Eboot -Destination (Join-Path $commsRun 'EBOOT.PBP')
Set-Content -LiteralPath (Join-Path $commsRun 'comms-panel-review.flag') -Value '1'
$commsProcess=Start-Process -FilePath (Resolve-Path $Emulator).Path -ArgumentList ('"'+(Join-Path $commsRun 'EBOOT.PBP')+'"') -WorkingDirectory $commsRun -WindowStyle Hidden -PassThru
try {
 $commsReport=Join-Path $commsRun 'comms-panel-review.txt';$commsDeadline=(Get-Date).AddSeconds(90)
 while((Get-Date) -lt $commsDeadline){
  if((Test-Path -LiteralPath $commsReport)-and(Select-String -LiteralPath $commsReport -Pattern '^RESULT ' -Quiet)){break}
  if($commsProcess.HasExited){throw 'Comms Panel review exited early'}
  Start-Sleep -Milliseconds 250
 }
 Get-Content -LiteralPath $commsReport
 Write-Output "Review: $commsRun"
 if(!(Select-String -LiteralPath $commsReport -Pattern '^RESULT 0 failures$' -Quiet)){throw 'Comms Panel checks failed or timed out'}
} finally {if(!$commsProcess.HasExited){Stop-Process -Id $commsProcess.Id}}
