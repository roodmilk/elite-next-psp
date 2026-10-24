param([string]$Eboot="$PSScriptRoot/../outputs/rumours-candidate-EBOOT.PBP",[string]$Emulator="$PSScriptRoot/../../../work/ppsspp/PPSSPPWindows64.exe")
$ErrorActionPreference='Stop'
$rumourRun=Join-Path (Split-Path $PSScriptRoot -Parent) ('outputs/rumours-review-'+(Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
New-Item -ItemType Directory -Path $rumourRun | Out-Null
Copy-Item -LiteralPath $Eboot -Destination (Join-Path $rumourRun 'EBOOT.PBP')
Set-Content -LiteralPath (Join-Path $rumourRun 'rumours-review.flag') -Value '1'
$rumourProcess=Start-Process -FilePath (Resolve-Path -LiteralPath $Emulator).Path -ArgumentList ('"'+(Join-Path $rumourRun 'EBOOT.PBP')+'"') -WorkingDirectory $rumourRun -WindowStyle Hidden -PassThru
try {
 $rumourReport=Join-Path $rumourRun 'rumours-review.txt';$rumourDeadline=(Get-Date).AddSeconds(60)
 while((Get-Date) -lt $rumourDeadline){
  if((Test-Path -LiteralPath $rumourReport)-and(Select-String -LiteralPath $rumourReport -Pattern '^RESULT ' -Quiet)){break}
  Start-Sleep -Milliseconds 250
 }
 Get-Content -LiteralPath $rumourReport
 Write-Output "Review: $rumourRun"
 if(!(Select-String -LiteralPath $rumourReport -Pattern '^RESULT 0 failures$' -Quiet)){throw 'RUMOURS checks failed or timed out'}
} finally {if(!$rumourProcess.HasExited){Stop-Process -Id $rumourProcess.Id}}
