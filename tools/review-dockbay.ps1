param([string]$Eboot="$PSScriptRoot/../outputs/dockbay398/review-build/EBOOT.PBP",[string]$Flag='dockbay-review',[string]$Report='', [string]$Music='', [int]$Model=1,[int]$Seconds=120,[int]$FlagValue=1)
$ErrorActionPreference='Stop'
$erlRoot=Split-Path $PSScriptRoot -Parent
$erlRun=Join-Path $erlRoot ('outputs/dockbay398/run-'+$Flag+'-'+(Get-Date -Format 'HHmmss'))
New-Item -ItemType Directory -Path $erlRun -Force | Out-Null
Copy-Item -LiteralPath $Eboot -Destination (Join-Path $erlRun 'EBOOT.PBP')
Set-Content -LiteralPath (Join-Path $erlRun ($Flag+'.flag')) -Value $FlagValue
if($Music){foreach($erlStation in @('Sol Rock','Elite Exploration')){$erlMusicFolder=Join-Path $erlRun ('music/'+$erlStation);New-Item -ItemType Directory -Path $erlMusicFolder -Force | Out-Null;Copy-Item -LiteralPath $Music -Destination (Join-Path $erlMusicFolder 'Secret Agent Rock.mp3')};Set-Content -LiteralPath (Join-Path $erlRun 'race-audio.flag') -Value 1}
$erlEmu=[IO.Path]::GetFullPath((Join-Path $erlRoot '../../work/ppsspp/PPSSPPWindows64.exe'))
$erlIni=[IO.File]::ReadAllText((Join-Path (Split-Path $erlEmu -Parent) 'memstick/PSP/SYSTEM/ppsspp.ini'))
$erlIni=$erlIni.Replace('IgnoreBadMemAccess = True','IgnoreBadMemAccess = False')
$erlIni=[regex]::Replace($erlIni,'GraphicsBackend = .*','GraphicsBackend = 2 (DIRECT3D11)')
$erlIni=[regex]::Replace($erlIni,'PSPModel = \d',"PSPModel = $Model")
$erlConfig=Join-Path $erlRun 'isolated.ini'
[IO.File]::WriteAllText($erlConfig,$erlIni)
$erlProcess=Start-Process -FilePath $erlEmu -ArgumentList @('--config="'+$erlConfig+'"','--log="'+(Join-Path $erlRun 'emulator.log')+'"','"'+(Join-Path $erlRun 'EBOOT.PBP')+'"') -WorkingDirectory $erlRun -WindowStyle Hidden -PassThru
try {
 if(!$Report){$Report=$Flag};$erlDeadline=(Get-Date).AddSeconds($Seconds);$erlReport=Join-Path $erlRun ($Report+'.txt')
 while((Get-Date) -lt $erlDeadline){if((Test-Path -LiteralPath $erlReport)-and(Select-String -LiteralPath $erlReport -Pattern '^RESULT ' -Quiet)){break};if($erlProcess.HasExited){break};Start-Sleep -Milliseconds 250}
 if(Test-Path -LiteralPath $erlReport){Get-Content -LiteralPath $erlReport}else{Get-Content -LiteralPath (Join-Path $erlRun 'emulator.log') -Tail 15}
 Write-Output "REVIEW: $erlRun"
}finally{if(!$erlProcess.HasExited){Stop-Process -Id $erlProcess.Id}}
