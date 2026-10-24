param([string]$Eboot="$PSScriptRoot/../EBOOT.PBP",[string]$Emulator="$PSScriptRoot/../../../work/ppsspp/PPSSPPWindows64.exe",[ValidateSet('progress','dossier')][string]$Kind='progress')
$ErrorActionPreference='Stop'
$codexStem=if($Kind -eq 'progress'){'codex-progress-review'}else{'codex-dossier-review'}
$codexRun=Join-Path (Split-Path $PSScriptRoot -Parent) ('outputs/'+$codexStem+'-'+(Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
New-Item -ItemType Directory -Path $codexRun | Out-Null
Copy-Item -LiteralPath $Eboot -Destination (Join-Path $codexRun 'EBOOT.PBP')
Set-Content -LiteralPath (Join-Path $codexRun ($codexStem+'.flag')) -Value 1
$codexProcess=Start-Process -FilePath (Resolve-Path -LiteralPath $Emulator).Path -ArgumentList ('"'+(Join-Path $codexRun 'EBOOT.PBP')+'"') -WorkingDirectory $codexRun -WindowStyle Hidden -PassThru
try {
 $codexReport=Join-Path $codexRun ($codexStem+'.txt');$codexDeadline=(Get-Date).AddSeconds(90)
 while((Get-Date) -lt $codexDeadline){if((Test-Path -LiteralPath $codexReport)-and(Select-String -LiteralPath $codexReport -Pattern '^RESULT ' -Quiet)){break};if($codexProcess.HasExited){throw 'Codex review exited early'};Start-Sleep -Milliseconds 250}
 Get-Content -LiteralPath $codexReport;Write-Output "Review: $codexRun"
 if(!(Select-String -LiteralPath $codexReport -Pattern '^RESULT 0 failures$' -Quiet)){throw 'Codex checks failed or timed out'}
} finally {if(!$codexProcess.HasExited){Stop-Process -Id $codexProcess.Id}}
