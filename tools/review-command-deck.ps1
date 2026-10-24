param([string]$Eboot="$PSScriptRoot/../EBOOT.PBP",[string]$Emulator="$PSScriptRoot/../../../work/ppsspp/PPSSPPWindows64.exe")
$ErrorActionPreference='Stop'
$deckRun=Join-Path (Split-Path $PSScriptRoot -Parent) ('outputs/deck-review-'+(Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
New-Item -ItemType Directory -Path $deckRun | Out-Null
Copy-Item -LiteralPath $Eboot -Destination (Join-Path $deckRun 'EBOOT.PBP')
Set-Content -LiteralPath (Join-Path $deckRun 'command-deck-review.flag') -Value '1'
$deckProcess=Start-Process -FilePath (Resolve-Path -LiteralPath $Emulator).Path -ArgumentList ('"'+(Join-Path $deckRun 'EBOOT.PBP')+'"') -WorkingDirectory $deckRun -WindowStyle Hidden -PassThru
try {
 $deckReport=Join-Path $deckRun 'command-deck-review.txt';$deckDeadline=(Get-Date).AddSeconds(180)
 while((Get-Date) -lt $deckDeadline){if((Test-Path -LiteralPath $deckReport)-and(Select-String -LiteralPath $deckReport -Pattern '^RESULT ' -Quiet)){break};Start-Sleep -Milliseconds 250}
 Get-Content -LiteralPath $deckReport
 Write-Output "Review: $deckRun"
 if(!(Select-String -LiteralPath $deckReport -Pattern '^RESULT 0 failures$' -Quiet)){throw 'Command Deck checks failed or timed out'}
}finally{if(!$deckProcess.HasExited){Stop-Process -Id $deckProcess.Id}}
