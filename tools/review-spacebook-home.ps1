param([string]$Eboot="$PSScriptRoot/../EBOOT.PBP",[string]$Emulator="$PSScriptRoot/../../../work/ppsspp/PPSSPPWindows64.exe")
$ErrorActionPreference='Stop'
$bookRun=Join-Path (Split-Path $PSScriptRoot -Parent) ('outputs/spacebook-home-'+(Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
New-Item -ItemType Directory -Path $bookRun | Out-Null
Copy-Item -LiteralPath $Eboot -Destination (Join-Path $bookRun 'EBOOT.PBP')
Set-Content -LiteralPath (Join-Path $bookRun 'spacebook-home-review.flag') -Value '1'
$bookProcess=Start-Process -FilePath (Resolve-Path -LiteralPath $Emulator).Path -ArgumentList ('"'+(Join-Path $bookRun 'EBOOT.PBP')+'"') -WorkingDirectory $bookRun -WindowStyle Hidden -PassThru
try {
 $bookReport=Join-Path $bookRun 'spacebook-home-review.txt';$bookDeadline=(Get-Date).AddSeconds(60)
 while((Get-Date) -lt $bookDeadline){
  if((Test-Path -LiteralPath $bookReport)-and(Select-String -LiteralPath $bookReport -Pattern '^RESULT ' -Quiet)){break}
  Start-Sleep -Milliseconds 250
 }
 Get-Content -LiteralPath $bookReport
 Write-Output "Review: $bookRun"
 if(!(Select-String -LiteralPath $bookReport -Pattern '^RESULT 0 failures$' -Quiet)){throw 'Spacebook home checks failed or timed out'}
} finally {if(!$bookProcess.HasExited){Stop-Process -Id $bookProcess.Id}}
