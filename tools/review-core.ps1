param(
    [string]$Eboot="$PSScriptRoot/../EBOOT.PBP",
    [string]$Emulator="$PSScriptRoot/../../../work/ppsspp/PPSSPPWindows64.exe",
    [int]$TimeoutSeconds=120
)
$ErrorActionPreference='Stop'
$reviewDir=Join-Path $PSScriptRoot ('../../../work/core-'+(Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
New-Item -ItemType Directory -Path $reviewDir | Out-Null
$reviewDir=(Resolve-Path -LiteralPath $reviewDir).Path
Copy-Item -LiteralPath $Eboot -Destination (Join-Path $reviewDir 'EBOOT.PBP')
Set-Content -LiteralPath (Join-Path $reviewDir 'core-check.flag') -Value '1'
$process=Start-Process -FilePath (Resolve-Path -LiteralPath $Emulator).Path -ArgumentList ('"'+(Join-Path $reviewDir 'EBOOT.PBP')+'"') -WorkingDirectory $reviewDir -WindowStyle Hidden -PassThru
try {
    $deadline=(Get-Date).AddSeconds($TimeoutSeconds);$final=Join-Path $reviewDir 'game-check.txt'
    while((Get-Date) -lt $deadline){if((Test-Path -LiteralPath $final)-and(Select-String -LiteralPath $final -Pattern '^RESULT ' -Quiet)){break};Start-Sleep -Milliseconds 250}
    foreach($name in @('game','radio','steering')){$path=Join-Path $reviewDir ($name+'-check.txt');if(!(Test-Path -LiteralPath $path)){throw "Missing $name report"};if(!(Select-String -LiteralPath $path -Pattern '^RESULT 0 failures$' -Quiet)-or(Select-String -LiteralPath $path -Pattern '^FAIL ' -Quiet)){throw "Failed $name checks: $path"};$count=(Select-String -LiteralPath $path -Pattern '^PASS ').Count;Write-Output "$name $count checks passed"}
    Write-Output "Review: $reviewDir"
} finally {if(!$process.HasExited){Stop-Process -Id $process.Id}}
