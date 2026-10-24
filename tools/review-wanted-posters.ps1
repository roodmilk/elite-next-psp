param([string]$Eboot="$PSScriptRoot/../EBOOT.PBP",[string]$Emulator="$PSScriptRoot/../../../work/ppsspp/PPSSPPWindows64.exe")
$ErrorActionPreference='Stop'
$wantedRun=Join-Path (Split-Path $PSScriptRoot -Parent) ('outputs/wanted-poster-'+(Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
New-Item -ItemType Directory -Path $wantedRun | Out-Null
Copy-Item -LiteralPath $Eboot -Destination (Join-Path $wantedRun 'EBOOT.PBP')
Set-Content -LiteralPath (Join-Path $wantedRun 'wanted-poster-review.flag') -Value '1'
$wantedProcess=Start-Process -FilePath (Resolve-Path -LiteralPath $Emulator).Path -ArgumentList ('"'+(Join-Path $wantedRun 'EBOOT.PBP')+'"') -WorkingDirectory $wantedRun -WindowStyle Hidden -PassThru
try {
 $wantedReport=Join-Path $wantedRun 'wanted-poster-review.txt';$wantedDeadline=(Get-Date).AddSeconds(60)
 while((Get-Date) -lt $wantedDeadline){
  if((Test-Path -LiteralPath $wantedReport)-and(Select-String -LiteralPath $wantedReport -Pattern '^RESULT ' -Quiet)){break}
  Start-Sleep -Milliseconds 250
 }
 Get-Content -LiteralPath $wantedReport
 Write-Output "Review: $wantedRun"
 if(!(Select-String -LiteralPath $wantedReport -Pattern '^RESULT 0 failures$' -Quiet)){throw 'Wanted poster checks failed or timed out'}
} finally {if(!$wantedProcess.HasExited){Stop-Process -Id $wantedProcess.Id}}
