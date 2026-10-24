param([string]$Eboot="$PSScriptRoot/../EBOOT.PBP",[string]$Emulator="$PSScriptRoot/../../../work/ppsspp/PPSSPPWindows64.exe")
$ErrorActionPreference='Stop'
$reviewRoot=Split-Path $PSScriptRoot -Parent
$reviewRun=Join-Path $reviewRoot ('outputs/exterior-review-'+(Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
New-Item -ItemType Directory -Path $reviewRun | Out-Null
Copy-Item -LiteralPath $Eboot -Destination (Join-Path $reviewRun 'EBOOT.PBP')
[IO.File]::WriteAllText((Join-Path $reviewRun 'exterior-review.flag'),'1')
$reviewProcess=Start-Process -FilePath (Resolve-Path -LiteralPath $Emulator).Path -ArgumentList ('"'+(Join-Path $reviewRun 'EBOOT.PBP')+'"') -WorkingDirectory $reviewRun -WindowStyle Hidden -PassThru
try {
 $reviewReport=Join-Path $reviewRun 'exterior-review.txt';$reviewDeadline=(Get-Date).AddSeconds(60)
 while((Get-Date) -lt $reviewDeadline){
  if((Test-Path -LiteralPath $reviewReport)-and(Select-String -LiteralPath $reviewReport -Pattern '^RESULT ' -Quiet)){break}
  Start-Sleep -Milliseconds 250
 }
 if(!(Test-Path -LiteralPath $reviewReport)){throw 'No exterior review report produced'}
 Get-Content -LiteralPath $reviewReport
 Write-Output "Review: $reviewRun"
 if(!(Select-String -LiteralPath $reviewReport -Pattern '^RESULT 0 failures$' -Quiet)){throw 'Exterior review failed or timed out'}
} finally {if(!$reviewProcess.HasExited){Stop-Process -Id $reviewProcess.Id}}
