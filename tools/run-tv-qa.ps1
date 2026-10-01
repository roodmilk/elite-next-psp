param([string]$Eboot,[string]$Mode='tv')
$ErrorActionPreference='Stop'
$root=Split-Path -Parent $PSScriptRoot
$dir=Join-Path $root ('qa-'+$Mode+'-'+(Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
New-Item -ItemType Directory -Path $dir | Out-Null
Copy-Item -LiteralPath $Eboot -Destination (Join-Path $dir 'EBOOT.PBP')
# Test-fixture files only, never copied into a player package.
if($Mode -eq 'smoke') { [IO.File]::WriteAllText((Join-Path $dir 'smoke.flag'),'1');$report='performance-check.txt' }
else { [IO.File]::WriteAllText((Join-Path $dir 'tv-check.flag'),'1');$report='local-tv-check.txt' }
[IO.File]::WriteAllText((Join-Path $dir 'headless.flag'),'1')
$emu='C:/Users/skarm/Documents/Codex/2026-09-14/let/work/ppsspp/PPSSPPWindows64.exe'
$p=Start-Process -FilePath $emu -ArgumentList ('"'+(Join-Path $dir 'EBOOT.PBP')+'"') -WorkingDirectory $dir -WindowStyle Hidden -PassThru
try {
 $deadline=(Get-Date).AddSeconds(600)
 do {
  Start-Sleep -Milliseconds 250
  $ready=(Test-Path -LiteralPath (Join-Path $dir $report)) -and (Select-String -LiteralPath (Join-Path $dir $report) -Pattern '^RESULT ' -Quiet)
 } while(!$ready -and !$p.HasExited -and (Get-Date) -lt $deadline)
 if(!$ready){throw "QA did not complete: $dir"}
 Get-ChildItem -LiteralPath $dir -Filter '*-check.txt' | ForEach-Object { $_.Name; Select-String -LiteralPath $_.FullName -Pattern '^RESULT |^FAIL ' | ForEach-Object Line }
 "Reports: $dir"
} finally {if(!$p.HasExited){Stop-Process -Id $p.Id}}
