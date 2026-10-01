param([Parameter(Mandatory=$true)][string]$Eboot,[string]$Emulator="$PSScriptRoot/../../../work/ppsspp/PPSSPPWindows64.exe")
$ErrorActionPreference='Stop'
$skyRoot=Split-Path $PSScriptRoot -Parent
$skyDir=Join-Path $skyRoot ('outputs/soft-sky-smoke-'+(Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
New-Item -ItemType Directory -Path $skyDir | Out-Null
Copy-Item -LiteralPath $Eboot -Destination (Join-Path $skyDir 'EBOOT.PBP')
[IO.File]::WriteAllText((Join-Path $skyDir 'smoke.flag'),'1')
$skyProcess=Start-Process -FilePath (Resolve-Path -LiteralPath $Emulator).Path -ArgumentList ('"'+(Join-Path $skyDir 'EBOOT.PBP')+'"') -WorkingDirectory $skyDir -WindowStyle Hidden -PassThru
try {
 $skyDeadline=(Get-Date).AddSeconds(120)
 while((Get-Date) -lt $skyDeadline){
  $skyPerf=Join-Path $skyDir 'performance-check.txt'
  if((Test-Path -LiteralPath $skyPerf) -and (Select-String -LiteralPath $skyPerf -Pattern '^RESULT ' -Quiet)){break}
  Start-Sleep -Milliseconds 250
 }
 $skyFailures=0
 foreach($skyGroup in @('game','input','steering','radio','performance')){
  $skyReport=Join-Path $skyDir ($skyGroup+'-check.txt')
  Write-Output "Group: $skyGroup"
  if(!(Test-Path -LiteralPath $skyReport)){Write-Output 'INCOMPLETE: missing report';$skyFailures++;continue}
  Select-String -LiteralPath $skyReport -Pattern '^(FAIL|RESULT) ' | ForEach-Object {Write-Output $_.Line}
  if(!(Select-String -LiteralPath $skyReport -Pattern '^RESULT 0 failures$' -Quiet)){$skyFailures++;Get-Content -LiteralPath $skyReport -Tail 3}
 }
 Write-Output "Smoke reports: $skyDir"
 if($skyFailures){throw "$skyFailures broad-suite groups failed or remained incomplete; preserve reports for diagnosis"}
} finally {if(!$skyProcess.HasExited){Stop-Process -Id $skyProcess.Id}}
