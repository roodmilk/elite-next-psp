param(
    [string]$Eboot="$PSScriptRoot/../EBOOT.PBP",
    [string]$Emulator="$PSScriptRoot/../../../work/ppsspp/PPSSPPWindows64.exe"
)
$ErrorActionPreference='Stop'
$reviewDir=Join-Path $PSScriptRoot ('../../../work/tool-feedback-'+(Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
New-Item -ItemType Directory -Path $reviewDir | Out-Null
$reviewDir=(Resolve-Path -LiteralPath $reviewDir).Path
Copy-Item -LiteralPath $Eboot -Destination (Join-Path $reviewDir 'EBOOT.PBP')
Set-Content -LiteralPath (Join-Path $reviewDir 'tool-feedback-check.flag') -Value '1'
$process=Start-Process -FilePath (Resolve-Path -LiteralPath $Emulator).Path -ArgumentList ('"'+(Join-Path $reviewDir 'EBOOT.PBP')+'"') -WorkingDirectory $reviewDir -WindowStyle Hidden -PassThru
try {
    $report=Join-Path $reviewDir 'tool-feedback-check.txt';$deadline=(Get-Date).AddSeconds(45)
    while((Get-Date) -lt $deadline){if((Test-Path -LiteralPath $report)-and(Select-String -LiteralPath $report -Pattern '^RESULT ' -Quiet)){break};Start-Sleep -Milliseconds 200}
    if(!(Test-Path -LiteralPath $report)){throw "Missing tool feedback report in $reviewDir"}
    Get-Content -LiteralPath $report
    if(!(Select-String -LiteralPath $report -Pattern '^RESULT 0 failures$' -Quiet)){throw "Tool feedback checks failed; see $report"}
    Write-Output "Review: $reviewDir"
} finally {if(!$process.HasExited){Stop-Process -Id $process.Id}}
