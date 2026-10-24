param([string]$Eboot="$PSScriptRoot/../outputs/newspapers399/stage-c-build/EBOOT.PBP",[string]$Flag='newspaper-review',[string]$Report='', [string]$Music='', [int]$Model=1,[int]$Seconds=120,[int]$FlagValue=0)
$ErrorActionPreference='Stop'
$newsRoot=Split-Path $PSScriptRoot -Parent
$newsRun=Join-Path $newsRoot ('outputs/newspapers399/run-'+$Flag+'-'+(Get-Date -Format 'HHmmss'))
New-Item -ItemType Directory -Path $newsRun -Force | Out-Null
Copy-Item -LiteralPath $Eboot -Destination (Join-Path $newsRun 'EBOOT.PBP')
Set-Content -LiteralPath (Join-Path $newsRun ($Flag+'.flag')) -Value $FlagValue
if($Music){foreach($newsStation in @('Sol Rock','Elite Exploration')){$newsMusicFolder=Join-Path $newsRun ('music/'+$newsStation);New-Item -ItemType Directory -Path $newsMusicFolder -Force | Out-Null;Copy-Item -LiteralPath $Music -Destination (Join-Path $newsMusicFolder 'Secret Agent Rock.mp3')};Set-Content -LiteralPath (Join-Path $newsRun 'race-audio.flag') -Value 1}
$newsEmu=[IO.Path]::GetFullPath((Join-Path $newsRoot '../../work/ppsspp/PPSSPPWindows64.exe'))
$newsIni=[IO.File]::ReadAllText((Join-Path (Split-Path $newsEmu -Parent) 'memstick/PSP/SYSTEM/ppsspp.ini'))
$newsIni=$newsIni.Replace('IgnoreBadMemAccess = True','IgnoreBadMemAccess = False')
$newsIni=[regex]::Replace($newsIni,'GraphicsBackend = .*','GraphicsBackend = 2 (DIRECT3D11)')
$newsIni=[regex]::Replace($newsIni,'PSPModel = \d',"PSPModel = $Model")
$newsConfig=Join-Path $newsRun 'isolated.ini'
[IO.File]::WriteAllText($newsConfig,$newsIni)
$newsProcess=Start-Process -FilePath $newsEmu -ArgumentList @('--config="'+$newsConfig+'"','--log="'+(Join-Path $newsRun 'emulator.log')+'"','"'+(Join-Path $newsRun 'EBOOT.PBP')+'"') -WorkingDirectory $newsRun -WindowStyle Hidden -PassThru
try {
 if(!$Report){$Report=$Flag};$newsDeadline=(Get-Date).AddSeconds($Seconds);$newsReport=Join-Path $newsRun ($Report+'.txt')
 while((Get-Date) -lt $newsDeadline){if((Test-Path -LiteralPath $newsReport)-and(Select-String -LiteralPath $newsReport -Pattern '^RESULT ' -Quiet)){break};if($newsProcess.HasExited){break};Start-Sleep -Milliseconds 250}
 if(Test-Path -LiteralPath $newsReport){Get-Content -LiteralPath $newsReport}else{Get-Content -LiteralPath (Join-Path $newsRun 'emulator.log') -Tail 15}
 Write-Output "REVIEW: $newsRun"
}finally{if(!$newsProcess.HasExited){Stop-Process -Id $newsProcess.Id}}
