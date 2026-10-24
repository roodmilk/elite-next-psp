param([string]$Eboot="$PSScriptRoot/../EBOOT.PBP",[string]$Emulator="$PSScriptRoot/../../../work/ppsspp/PPSSPPWindows64.exe")
$ErrorActionPreference='Stop'
$regionRun=Join-Path (Split-Path $PSScriptRoot -Parent) ('outputs/planet-regions-'+(Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
New-Item -ItemType Directory -Path $regionRun | Out-Null
Copy-Item -LiteralPath $Eboot -Destination (Join-Path $regionRun 'EBOOT.PBP')
Set-Content -LiteralPath (Join-Path $regionRun 'planet-regions-review.flag') -Value '1'
Write-Output "Review: $regionRun"
$regionProcess=Start-Process -FilePath (Resolve-Path -LiteralPath $Emulator).Path -ArgumentList @('--log="'+(Join-Path $regionRun 'emulator.log')+'"','"'+(Join-Path $regionRun 'EBOOT.PBP')+'"') -WorkingDirectory $regionRun -WindowStyle Hidden -PassThru
try {
 $regionReport=Join-Path $regionRun 'planet-regions-review.txt';$regionDeadline=(Get-Date).AddSeconds(240)
 while((Get-Date) -lt $regionDeadline){if((Test-Path -LiteralPath $regionReport)-and(Select-String -LiteralPath $regionReport -Pattern '^RESULT ' -Quiet)){break};if($regionProcess.HasExited){throw "Emulator exited before completing $regionReport; see emulator.log"};Start-Sleep -Milliseconds 250}
 Get-Content -LiteralPath $regionReport
 if(!(Select-String -LiteralPath $regionReport -Pattern '^RESULT 0 failures$' -Quiet)){throw 'Planet region checks failed or timed out'}
}finally{if(!$regionProcess.HasExited){Stop-Process -Id $regionProcess.Id}}
