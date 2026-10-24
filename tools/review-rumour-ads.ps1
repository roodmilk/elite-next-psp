param([Parameter(Mandatory=$true)][string]$Eboot,[ValidateSet('rumour-ads-review','rumours-review')][string]$Suite='rumour-ads-review')
$ErrorActionPreference='Stop'
$adRoot=Split-Path $PSScriptRoot -Parent
$adRun=Join-Path $adRoot ('outputs/rumour-ads-'+$Suite+'-'+(Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
New-Item -ItemType Directory -Path $adRun | Out-Null
Copy-Item -LiteralPath $Eboot -Destination (Join-Path $adRun 'EBOOT.PBP')
Set-Content -LiteralPath (Join-Path $adRun ($Suite+'.flag')) -Value 1
$adEmulator=(Resolve-Path (Join-Path $adRoot '../../work/ppsspp/PPSSPPWindows64.exe')).Path
$adIni=[IO.File]::ReadAllText((Join-Path (Split-Path $adEmulator -Parent) 'memstick/PSP/SYSTEM/ppsspp.ini'))
$adIni=$adIni.Replace('IgnoreBadMemAccess = True','IgnoreBadMemAccess = False')
$adConfig=Join-Path $adRun 'isolated.ini';[IO.File]::WriteAllText($adConfig,$adIni)
$adProcess=Start-Process -FilePath $adEmulator -ArgumentList @('--config="'+$adConfig+'"','--log="'+(Join-Path $adRun 'emulator.log')+'"','"'+(Join-Path $adRun 'EBOOT.PBP')+'"') -WorkingDirectory $adRun -WindowStyle Hidden -PassThru
try {
 $adReport=Join-Path $adRun ($Suite+'.txt');$adDeadline=(Get-Date).AddSeconds(180)
 while((Get-Date) -lt $adDeadline){
  if((Test-Path -LiteralPath $adReport)-and(Select-String -LiteralPath $adReport -Pattern '^RESULT ' -Quiet)){break}
  if($adProcess.HasExited){break}
  Start-Sleep -Milliseconds 250
 }
 Get-Content -LiteralPath $adReport
 Write-Output "Review: $adRun"
 if(!(Select-String -LiteralPath $adReport -Pattern '^RESULT 0 failures$' -Quiet)){throw "$Suite checks failed or timed out"}
}finally{if(!$adProcess.HasExited){Stop-Process -Id $adProcess.Id}}
