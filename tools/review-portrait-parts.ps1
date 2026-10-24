param([string]$Eboot="$PSScriptRoot/../EBOOT.PBP",[string]$Emulator="$PSScriptRoot/../../../work/ppsspp/PPSSPPWindows64.exe")
$ErrorActionPreference='Stop'
$portraitRun=Join-Path (Split-Path $PSScriptRoot -Parent) ('outputs/portrait-parts-'+(Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
New-Item -ItemType Directory -Path $portraitRun | Out-Null
Copy-Item -LiteralPath $Eboot -Destination (Join-Path $portraitRun 'EBOOT.PBP')
Set-Content -LiteralPath (Join-Path $portraitRun 'portrait-parts-review.flag') -Value '1'
$portraitProcess=Start-Process -FilePath (Resolve-Path -LiteralPath $Emulator).Path -ArgumentList ('"'+(Join-Path $portraitRun 'EBOOT.PBP')+'"') -WorkingDirectory $portraitRun -WindowStyle Hidden -PassThru
try {
 $portraitReport=Join-Path $portraitRun 'portrait-parts-review.txt';$portraitDeadline=(Get-Date).AddSeconds(300)
 while((Get-Date) -lt $portraitDeadline){if((Test-Path -LiteralPath $portraitReport)-and(Select-String -LiteralPath $portraitReport -Pattern '^RESULT ' -Quiet)){break};if($portraitProcess.HasExited){throw 'Portrait review exited early'};Start-Sleep -Milliseconds 250}
 Get-Content -LiteralPath $portraitReport
 Write-Output "Review: $portraitRun"
 if(!(Select-String -LiteralPath $portraitReport -Pattern '^RESULT 0 failures$' -Quiet)){throw 'Portrait part checks failed or timed out'}
} finally {if(!$portraitProcess.HasExited){Stop-Process -Id $portraitProcess.Id}}
