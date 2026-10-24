param([string]$Eboot="$PSScriptRoot/../EBOOT.PBP",[string]$Emulator="$PSScriptRoot/../../../work/ppsspp/PPSSPPWindows64.exe")
$ErrorActionPreference='Stop'
$fixRun=Join-Path (Split-Path $PSScriptRoot -Parent) ('outputs/menu-fixes-review-'+(Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
New-Item -ItemType Directory -Path $fixRun | Out-Null
Copy-Item -LiteralPath $Eboot -Destination (Join-Path $fixRun 'EBOOT.PBP')
Set-Content -LiteralPath (Join-Path $fixRun 'menu-fixes-review.flag') -Value 1
$fixProcess=Start-Process -FilePath (Resolve-Path -LiteralPath $Emulator).Path -ArgumentList ('"'+(Join-Path $fixRun 'EBOOT.PBP')+'"') -WorkingDirectory $fixRun -WindowStyle Hidden -PassThru
try {
 $fixReport=Join-Path $fixRun 'menu-fixes-review.txt';$fixDeadline=(Get-Date).AddSeconds(120)
 while((Get-Date) -lt $fixDeadline){if((Test-Path -LiteralPath $fixReport)-and(Select-String -LiteralPath $fixReport -Pattern '^RESULT ' -Quiet)){break};if($fixProcess.HasExited){throw 'Menu review exited early'};Start-Sleep -Milliseconds 250}
 Get-Content -LiteralPath $fixReport;Write-Output "Review: $fixRun"
 if(!(Select-String -LiteralPath $fixReport -Pattern '^RESULT 0 failures$' -Quiet)){throw 'Menu fix checks failed or timed out'}
} finally {if(!$fixProcess.HasExited){Stop-Process -Id $fixProcess.Id}}
