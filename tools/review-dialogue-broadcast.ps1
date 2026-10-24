param([string]$Eboot="$PSScriptRoot/../outputs/review-build/EBOOT.PBP",[string]$Flag='dialogue-broadcast-review')
$ErrorActionPreference='Stop'
$broadcastRun=Join-Path (Split-Path $PSScriptRoot -Parent) ('outputs/'+$Flag+'-'+(Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
New-Item -ItemType Directory -Path $broadcastRun | Out-Null
Copy-Item -LiteralPath $Eboot -Destination (Join-Path $broadcastRun 'EBOOT.PBP')
Set-Content -LiteralPath (Join-Path $broadcastRun ($Flag+'.flag')) -Value 1
$broadcastEmulator=(Resolve-Path "$PSScriptRoot/../../../work/ppsspp/PPSSPPWindows64.exe").Path
$broadcastConfig=Join-Path $broadcastRun 'isolated.ini'
$broadcastIni=[IO.File]::ReadAllText((Join-Path (Split-Path $broadcastEmulator -Parent) 'memstick/PSP/SYSTEM/ppsspp.ini'))
$broadcastIni=$broadcastIni.Replace('IgnoreBadMemAccess = True','IgnoreBadMemAccess = False')
[IO.File]::WriteAllText($broadcastConfig,$broadcastIni)
$broadcastProcess=Start-Process -FilePath $broadcastEmulator -ArgumentList @('--config="'+$broadcastConfig+'"','--log="'+(Join-Path $broadcastRun 'emulator.log')+'"','"'+(Join-Path $broadcastRun 'EBOOT.PBP')+'"') -WorkingDirectory $broadcastRun -WindowStyle Hidden -PassThru
try {
 $broadcastReport=Join-Path $broadcastRun ($Flag+'.txt');$broadcastDeadline=(Get-Date).AddSeconds(240)
 while((Get-Date) -lt $broadcastDeadline){
  if((Test-Path $broadcastReport)-and(Select-String -LiteralPath $broadcastReport -Pattern '^RESULT ' -Quiet)){break}
  if($broadcastProcess.HasExited){break};Start-Sleep -Milliseconds 250
 }
 Get-Content -LiteralPath $broadcastReport
 Write-Output "Review: $broadcastRun"
 if(!(Select-String -LiteralPath $broadcastReport -Pattern '^RESULT 0 failures$' -Quiet)){throw 'Broadcast review failed or timed out'}
}finally{if(!$broadcastProcess.HasExited){Stop-Process -Id $broadcastProcess.Id}}
