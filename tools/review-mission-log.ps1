param([string]$Eboot="$PSScriptRoot/../EBOOT.PBP",[string]$Emulator="$PSScriptRoot/../../../work/ppsspp/PPSSPPWindows64.exe")
$ErrorActionPreference='Stop'
$missionRun=Join-Path (Split-Path $PSScriptRoot -Parent) ('outputs/mission-log-review-'+(Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
New-Item -ItemType Directory -Path $missionRun | Out-Null
Copy-Item -LiteralPath $Eboot -Destination (Join-Path $missionRun 'EBOOT.PBP')
Set-Content -LiteralPath (Join-Path $missionRun 'mission-log-review.flag') -Value '1'
$missionProcess=Start-Process -FilePath (Resolve-Path -LiteralPath $Emulator).Path -ArgumentList ('"'+(Join-Path $missionRun 'EBOOT.PBP')+'"') -WorkingDirectory $missionRun -WindowStyle Hidden -PassThru
try {
 $missionReport=Join-Path $missionRun 'mission-log-review.txt';$missionDeadline=(Get-Date).AddSeconds(60)
 while((Get-Date) -lt $missionDeadline){
  if((Test-Path -LiteralPath $missionReport)-and(Select-String -LiteralPath $missionReport -Pattern '^RESULT ' -Quiet)){break}
  if($missionProcess.HasExited){throw 'Mission Log review exited early'}
  Start-Sleep -Milliseconds 250
 }
 Get-Content -LiteralPath $missionReport
 Write-Output "Review: $missionRun"
 if(!(Select-String -LiteralPath $missionReport -Pattern '^RESULT 0 failures$' -Quiet)){throw 'Mission Log checks failed or timed out'}
} finally {if(!$missionProcess.HasExited){Stop-Process -Id $missionProcess.Id}}
