param([string]$Eboot="$PSScriptRoot/../EBOOT.PBP",[string]$Emulator="$PSScriptRoot/../../../work/ppsspp/PPSSPPWindows64.exe")
$ErrorActionPreference='Stop'
$combatRun=Join-Path (Split-Path $PSScriptRoot -Parent) ('outputs/npc-combat-'+(Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
New-Item -ItemType Directory -Path $combatRun | Out-Null
Copy-Item -LiteralPath $Eboot -Destination (Join-Path $combatRun 'EBOOT.PBP')
Set-Content -LiteralPath (Join-Path $combatRun 'npc-combat-review.flag') -Value '1'
$combatProcess=Start-Process -FilePath (Resolve-Path -LiteralPath $Emulator).Path -ArgumentList ('"'+(Join-Path $combatRun 'EBOOT.PBP')+'"') -WorkingDirectory $combatRun -WindowStyle Hidden -PassThru
try {
 $combatReport=Join-Path $combatRun 'npc-combat-check.txt';$combatDeadline=(Get-Date).AddSeconds(60)
 while((Get-Date) -lt $combatDeadline){
  if((Test-Path -LiteralPath $combatReport)-and(Select-String -LiteralPath $combatReport -Pattern '^RESULT ' -Quiet)){break}
  Start-Sleep -Milliseconds 250
 }
 Get-Content -LiteralPath $combatReport
 Write-Output "Review: $combatRun"
 if(!(Select-String -LiteralPath $combatReport -Pattern '^RESULT 0 failures$' -Quiet)){throw 'NPC combat checks failed or timed out'}
} finally {if(!$combatProcess.HasExited){Stop-Process -Id $combatProcess.Id}}
