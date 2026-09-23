param([string]$Emulator="$PSScriptRoot/../../work/ppsspp/PPSSPPWindows64.exe")
$ErrorActionPreference='Stop'
$testDir=Join-Path $PSScriptRoot ('../../work/smoke-'+(Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
New-Item -ItemType Directory -Path $testDir | Out-Null
$testDir=(Resolve-Path -LiteralPath $testDir).Path
Copy-Item -LiteralPath "$PSScriptRoot/EBOOT.PBP" -Destination $testDir
if(Test-Path -LiteralPath "$PSScriptRoot/language.cfg"){Copy-Item -LiteralPath "$PSScriptRoot/language.cfg" -Destination $testDir}
if(Test-Path -LiteralPath "$PSScriptRoot/content.pack"){Copy-Item -LiteralPath "$PSScriptRoot/content.pack" -Destination $testDir}
if(Test-Path -LiteralPath "$PSScriptRoot/planet.content"){Copy-Item -LiteralPath "$PSScriptRoot/planet.content" -Destination $testDir}
Set-Content -LiteralPath (Join-Path $testDir 'smoke.flag') -Value '1'
$eboot=Join-Path $testDir 'EBOOT.PBP'
$process=Start-Process -FilePath (Resolve-Path -LiteralPath $Emulator).Path -ArgumentList ('"'+$eboot+'"') -WorkingDirectory $testDir -WindowStyle Hidden -PassThru
try {
    $deadline=(Get-Date).AddSeconds(45)
    $report=Join-Path $testDir 'performance-check.txt'
    while((Get-Date) -lt $deadline) {
        if((Test-Path -LiteralPath $report) -and (Select-String -LiteralPath $report -Pattern '^RESULT ' -Quiet)){break}
        Start-Sleep -Milliseconds 250
    }
    foreach($name in @('game','input','steering','radio','performance')) {
        $path=Join-Path $testDir ($name+'-check.txt')
        if(!(Test-Path -LiteralPath $path)){throw "Missing $name report in $testDir"}
        if(!(Select-String -LiteralPath $path -Pattern '^RESULT 0 failures$' -Quiet) -or (Select-String -LiteralPath $path -Pattern '^FAIL ' -Quiet)){throw "Failed $name checks; see $path"}
        Write-Output "$name checks passed"
    }
    Write-Output "Reports: $testDir"
} finally {
    # Stop only the emulator instance started for this disposable test.
    if(!$process.HasExited){Stop-Process -Id $process.Id}
}
