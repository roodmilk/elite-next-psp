param([string]$Eboot="$PSScriptRoot/../EBOOT.PBP",[string]$Emulator="$PSScriptRoot/../../../work/ppsspp/PPSSPPWindows64.exe",[switch]$CaptureAll)
$ErrorActionPreference='Stop'
$newsRun=Join-Path (Split-Path $PSScriptRoot -Parent) ('outputs/newspapers-'+(Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
New-Item -ItemType Directory -Path $newsRun | Out-Null
Copy-Item -LiteralPath $Eboot -Destination (Join-Path $newsRun 'EBOOT.PBP')
Set-Content -LiteralPath (Join-Path $newsRun 'newspaper-review.flag') -Value ([int]$CaptureAll.IsPresent)
$newsProcess=Start-Process -FilePath (Resolve-Path -LiteralPath $Emulator).Path -ArgumentList ('"'+(Join-Path $newsRun 'EBOOT.PBP')+'"') -WorkingDirectory $newsRun -WindowStyle Hidden -PassThru
try {
 $newsReport=Join-Path $newsRun 'newspaper-review.txt';$newsDeadline=(Get-Date).AddMinutes(10)
 while((Get-Date) -lt $newsDeadline){
  if((Test-Path -LiteralPath $newsReport)-and(Select-String -LiteralPath $newsReport -Pattern '^RESULT ' -Quiet)){break}
  Start-Sleep -Milliseconds 250
 }
 Get-Content -LiteralPath $newsReport
 Write-Output "Review: $newsRun"
 if(!(Select-String -LiteralPath $newsReport -Pattern '^RESULT 0 failures$' -Quiet)){throw 'Newspaper checks failed or timed out'}
} finally {if(!$newsProcess.HasExited){Stop-Process -Id $newsProcess.Id}}
