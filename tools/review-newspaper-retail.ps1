param([Parameter(Mandatory=$true)][string]$Eboot,[int]$System=7,[int]$Page=0)
$ErrorActionPreference='Stop'
$newsRoot=Split-Path $PSScriptRoot -Parent
$newsRun=Join-Path $newsRoot ('outputs/newspapers399/retail-s{0:D3}-p{1:D2}-' -f $System,$Page)
$newsRun+=(Get-Date -Format 'HHmmss')
New-Item -ItemType Directory -Path $newsRun -Force | Out-Null
Copy-Item -LiteralPath $Eboot -Destination (Join-Path $newsRun 'EBOOT.PBP')
Set-Content -LiteralPath (Join-Path $newsRun 'open-newspaper.flag') -Value "$System $Page"
Set-Content -LiteralPath (Join-Path $newsRun 'dump-native.flag') -Value 1
$newsEmu=[IO.Path]::GetFullPath((Join-Path $newsRoot '../../work/ppsspp/PPSSPPWindows64.exe'))
$newsIni=[IO.File]::ReadAllText((Join-Path (Split-Path $newsEmu -Parent) 'memstick/PSP/SYSTEM/ppsspp.ini'))
$newsIni=$newsIni.Replace('IgnoreBadMemAccess = True','IgnoreBadMemAccess = False')
$newsIni=[regex]::Replace($newsIni,'GraphicsBackend = .*','GraphicsBackend = 2 (DIRECT3D11)')
$newsIni=[regex]::Replace($newsIni,'PSPModel = \d','PSPModel = 0')
$newsConfig=Join-Path $newsRun 'isolated.ini';[IO.File]::WriteAllText($newsConfig,$newsIni)
$newsProcess=Start-Process -FilePath $newsEmu -ArgumentList @('--config="'+$newsConfig+'"','--log="'+(Join-Path $newsRun 'emulator.log')+'"','"'+(Join-Path $newsRun 'EBOOT.PBP')+'"') -WorkingDirectory $newsRun -WindowStyle Hidden -PassThru
try {
 $newsDeadline=(Get-Date).AddSeconds(30)
 while((Get-Date) -lt $newsDeadline -and !(Test-Path -LiteralPath (Join-Path $newsRun 'native-480x272.bmp'))){if($newsProcess.HasExited){throw 'Retail preview exited before rendering'};Start-Sleep -Milliseconds 100}
 Start-Sleep -Seconds 2
 & "$PSScriptRoot/review-psp-boot.ps1" -Eboot $Eboot -Model 0 -Scene home -ExistingRun $newsRun
 Write-Output "NEWSPAPER RETAIL: $newsRun"
}finally{if(!$newsProcess.HasExited){Stop-Process -Id $newsProcess.Id}}
