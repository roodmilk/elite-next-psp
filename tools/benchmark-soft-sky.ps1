param([Parameter(Mandatory=$true)][string]$Eboot,[string]$Emulator="$PSScriptRoot/../../../work/ppsspp/PPSSPPWindows64.exe")
$ErrorActionPreference='Stop'
$skyRoot=Split-Path $PSScriptRoot -Parent
$skyDir=Join-Path $skyRoot ('outputs/soft-sky-performance-'+(Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
New-Item -ItemType Directory -Path $skyDir | Out-Null
$skyExe=(Resolve-Path -LiteralPath $Emulator).Path
$skyCases=@(@(7,1),@(7,2),@(127,2),@(255,2),@(127,3))
$skyFailures=0
foreach($skyCase in $skyCases){
 $skySystem=$skyCase[0];$skyMode=$skyCase[1]
 $skyRun=Join-Path $skyDir "system-$skySystem-mode-$skyMode"
 New-Item -ItemType Directory -Path $skyRun | Out-Null
 Copy-Item -LiteralPath $Eboot -Destination (Join-Path $skyRun 'EBOOT.PBP')
 [IO.File]::WriteAllText((Join-Path $skyRun 'open-sky.flag'),[string]$skySystem)
 [IO.File]::WriteAllText((Join-Path $skyRun 'sky-benchmark.flag'),[string]$skyMode)
 $skyProcess=Start-Process -FilePath $skyExe -ArgumentList ('"'+(Join-Path $skyRun 'EBOOT.PBP')+'"') -WorkingDirectory $skyRun -WindowStyle Hidden -PassThru
 try {
  $skyDeadline=(Get-Date).AddSeconds(40);$skyReport=Join-Path $skyRun 'sky-performance.txt'
  while((Get-Date) -lt $skyDeadline){
   if((Test-Path -LiteralPath $skyReport) -and (Select-String -LiteralPath $skyReport -Pattern '^RESULT ' -Quiet)){break}
   Start-Sleep -Milliseconds 250
  }
  if(!(Test-Path -LiteralPath $skyReport)){throw "No report for $skyRun"}
  Get-Content -LiteralPath $skyReport
  if(!(Select-String -LiteralPath $skyReport -Pattern "^SYSTEM $skySystem .*mode=$skyMode;" -Quiet)){throw 'Wrong system or renderer in report'}
  if(!(Select-String -LiteralPath $skyReport -Pattern '^RESULT 0 failures$' -Quiet)){$skyFailures++;Write-Warning "Performance failed for $skyRun"}
 } finally {if(!$skyProcess.HasExited){Stop-Process -Id $skyProcess.Id}}
}
Write-Output "Performance: $skyDir"
if($skyFailures){throw "$skyFailures performance cases failed; see reports above"}
