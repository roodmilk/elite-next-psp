param([string]$Eboot="$PSScriptRoot/../EBOOT.PBP",[string]$Emulator="$PSScriptRoot/../../../work/ppsspp/PPSSPPWindows64.exe")
$ErrorActionPreference='Stop'
$cargoRun=Join-Path (Split-Path $PSScriptRoot -Parent) ('outputs/cargo-console-'+(Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
New-Item -ItemType Directory -Path $cargoRun | Out-Null
Copy-Item -LiteralPath $Eboot -Destination (Join-Path $cargoRun 'EBOOT.PBP')
Set-Content -LiteralPath (Join-Path $cargoRun 'cargo-console-review.flag') -Value '1'
$cargoProcess=Start-Process -FilePath (Resolve-Path -LiteralPath $Emulator).Path -ArgumentList ('"'+(Join-Path $cargoRun 'EBOOT.PBP')+'"') -WorkingDirectory $cargoRun -WindowStyle Hidden -PassThru
try {
 $cargoReport=Join-Path $cargoRun 'cargo-console-review.txt';$cargoDeadline=(Get-Date).AddSeconds(90)
 while((Get-Date) -lt $cargoDeadline){
  if((Test-Path -LiteralPath $cargoReport)-and(Select-String -LiteralPath $cargoReport -Pattern '^RESULT ' -Quiet)){break}
  Start-Sleep -Milliseconds 250
 }
 Get-Content -LiteralPath $cargoReport
 Write-Output "Review: $cargoRun"
 if(!(Select-String -LiteralPath $cargoReport -Pattern '^RESULT 0 failures$' -Quiet)){throw 'Cargo console checks failed or timed out'}
} finally {if(!$cargoProcess.HasExited){Stop-Process -Id $cargoProcess.Id}}
