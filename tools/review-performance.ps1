param([string]$Eboot="$PSScriptRoot/../EBOOT.PBP",[string]$Emulator="$PSScriptRoot/../../../work/ppsspp/PPSSPPWindows64.exe")
$ErrorActionPreference='Stop'
$perfRoot=Split-Path $PSScriptRoot -Parent
$perfRun=Join-Path $perfRoot ('outputs/performance-review-'+(Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
New-Item -ItemType Directory -Path $perfRun | Out-Null
Copy-Item -LiteralPath $Eboot -Destination (Join-Path $perfRun 'EBOOT.PBP')
[IO.File]::WriteAllText((Join-Path $perfRun 'performance-review.flag'),'1')
$perfExe=(Resolve-Path -LiteralPath $Emulator).Path
$perfProcess=Start-Process -FilePath $perfExe -ArgumentList ('"'+(Join-Path $perfRun 'EBOOT.PBP')+'"') -WorkingDirectory $perfRun -WindowStyle Hidden -PassThru
try {
 $perfReport=Join-Path $perfRun 'performance-review.txt';$perfDeadline=(Get-Date).AddSeconds(120)
 while((Get-Date) -lt $perfDeadline){
  if((Test-Path -LiteralPath $perfReport)-and(Select-String -LiteralPath $perfReport -Pattern '^RESULT ' -Quiet)){break}
  Start-Sleep -Milliseconds 250
 }
 if(!(Test-Path -LiteralPath $perfReport)){throw 'No performance report produced'}
 Get-Content -LiteralPath $perfReport
 Write-Output "Review: $perfRun"
 if(!(Select-String -LiteralPath $perfReport -Pattern '^RESULT 0 failures$' -Quiet)){throw 'Performance checks failed or timed out'}
} finally {if(!$perfProcess.HasExited){Stop-Process -Id $perfProcess.Id}}
