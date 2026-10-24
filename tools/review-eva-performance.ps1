param([string]$Eboot="$PSScriptRoot/../EBOOT.PBP",[string]$Emulator="$PSScriptRoot/../../../work/ppsspp/PPSSPPWindows64.exe",[string]$Review='eva-performance-review')
$ErrorActionPreference='Stop'
if($Review -notin @('eva-performance-review','planet-speed-review','surface-perf-review','starport-review','opening-check')){throw 'Unsupported review'}
$evaRoot=Split-Path $PSScriptRoot -Parent
$evaRun=Join-Path $evaRoot ('outputs/'+$Review+'-'+(Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
New-Item -ItemType Directory -Path $evaRun | Out-Null
Copy-Item -LiteralPath $Eboot -Destination (Join-Path $evaRun 'EBOOT.PBP')
[IO.File]::WriteAllText((Join-Path $evaRun ($Review+'.flag')),'1')
$evaProcess=Start-Process -FilePath (Resolve-Path -LiteralPath $Emulator).Path -ArgumentList ('"'+(Join-Path $evaRun 'EBOOT.PBP')+'"') -WorkingDirectory $evaRun -WindowStyle Hidden -PassThru
try {
 $evaReport=Join-Path $evaRun ($Review+'.txt');$evaTimeout=if($Review -eq 'opening-check'){300}else{120};$evaDeadline=(Get-Date).AddSeconds($evaTimeout)
 while((Get-Date) -lt $evaDeadline){
  if((Test-Path -LiteralPath $evaReport)-and(Select-String -LiteralPath $evaReport -Pattern '^RESULT ' -Quiet)){break}
  Start-Sleep -Milliseconds 250
 }
 if(!(Test-Path -LiteralPath $evaReport)){throw 'No EVA report produced'}
 Get-Content -LiteralPath $evaReport
 Write-Output "Review: $evaRun"
 if(!(Select-String -LiteralPath $evaReport -Pattern '^RESULT 0 failures$' -Quiet)){throw 'EVA review failed or timed out'}
} finally {if(!$evaProcess.HasExited){Stop-Process -Id $evaProcess.Id}}
