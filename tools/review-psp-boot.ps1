param([Parameter(Mandatory=$true)][string]$Eboot,[int]$Model=0,[string]$Music='', [ValidateSet('intro','home','flight','dream','erl-hq','erl-race')][string]$Scene='intro',[string]$ExistingRun='')
$ErrorActionPreference='Stop'
$bootRoot=Split-Path $PSScriptRoot -Parent
$bootProcess=$null
if(!$ExistingRun){
$bootRun=Join-Path $bootRoot ('outputs/psp-boot-'+$Model+'-'+$Scene+'-'+(Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
New-Item -ItemType Directory -Path $bootRun | Out-Null
Copy-Item -LiteralPath $Eboot -Destination (Join-Path $bootRun 'EBOOT.PBP')
Set-Content -LiteralPath (Join-Path $bootRun 'dump-native.flag') -Value 1
if($Scene -eq 'home'){Set-Content -LiteralPath (Join-Path $bootRun 'open-home.flag') -Value 0}
if($Scene -eq 'flight'){Set-Content -LiteralPath (Join-Path $bootRun 'open-flight.flag') -Value 1}
if($Scene -eq 'dream'){Set-Content -LiteralPath (Join-Path $bootRun 'dream-live.flag') -Value 1}
if($Scene -eq 'erl-hq'){Set-Content -LiteralPath (Join-Path $bootRun 'open-erl.flag') -Value 0}
if($Scene -eq 'erl-race'){Set-Content -LiteralPath (Join-Path $bootRun 'open-erl.flag') -Value 1}
if($Music){
 foreach($bootStation in @('Elite Exploration','Sol Rock')){
  $bootMusic=Join-Path $bootRun ('music/'+$bootStation)
  New-Item -ItemType Directory -Path $bootMusic -Force | Out-Null
  Copy-Item -LiteralPath $Music -Destination (Join-Path $bootMusic ([IO.Path]::GetFileName($Music)))
 }
}
$bootEmulator=(Resolve-Path "$PSScriptRoot/../../../work/ppsspp/PPSSPPWindows64.exe").Path
$bootIni=[IO.File]::ReadAllText((Join-Path (Split-Path $bootEmulator -Parent) 'memstick/PSP/SYSTEM/ppsspp.ini'))
$bootIni=[regex]::Replace($bootIni,'(?m)^PSPModel = \d+',"PSPModel = $Model")
$bootIni=$bootIni.Replace('IgnoreBadMemAccess = True','IgnoreBadMemAccess = False')
$bootIni=[regex]::Replace($bootIni,'(?m)^GraphicsBackend = .*','GraphicsBackend = 2 (DIRECT3D11)')
$bootIni=[regex]::Replace($bootIni,'(?m)^(ME|SCEMISC)Level = \d+','$1Level = 5')
$bootConfig=Join-Path $bootRun 'isolated.ini';[IO.File]::WriteAllText($bootConfig,$bootIni)
$bootLog=Join-Path $bootRun 'emulator.log'
$bootProcess=Start-Process -FilePath $bootEmulator -ArgumentList @('--config="'+$bootConfig+'"','--log="'+$bootLog+'"','"'+(Join-Path $bootRun 'EBOOT.PBP')+'"') -WorkingDirectory $bootRun -WindowStyle Hidden -PassThru
}else{
 $bootRun=(Resolve-Path -LiteralPath $ExistingRun).Path
 $bootLog=Join-Path $bootRun 'emulator.log'
 if((Get-FileHash -LiteralPath $Eboot).Hash -ne (Get-FileHash -LiteralPath (Join-Path $bootRun 'EBOOT.PBP')).Hash){throw 'Existing run used a different EBOOT'}
}
try{
 if($Scene -eq 'dream'){
  $bootDeadline=(Get-Date).AddSeconds(175)
  do {Start-Sleep -Milliseconds 250;$bootDone=Test-Path (Join-Path $bootRun 'dream-live-check.txt')} while(!$bootDone -and $bootProcess -and !$bootProcess.HasExited -and (Get-Date) -lt $bootDeadline)
  if(!$bootDone -or !(Select-String -LiteralPath (Join-Path $bootRun 'dream-live-check.txt') -Pattern '^RESULT 0 failures$' -Quiet)){throw 'Live dream failed'}
 }elseif(!$ExistingRun){Start-Sleep -Seconds 12;if($bootProcess.HasExited){throw 'Startup process exited unexpectedly'}}
 # The dream intentionally begins with closed eyes. Check the rendered battle
 # after the eyes have opened; retain the original cold frame as evidence.
 $bootFrame=Join-Path $bootRun $(if($Scene -eq 'dream'){'live-dream-281-10.bmp'}else{'native-480x272.bmp'})
 if(!(Test-Path $bootFrame)){throw 'No rendered frame'}
 $bootBytes=[IO.File]::ReadAllBytes($bootFrame)
 if($bootBytes.Length -ne 391734 -or [BitConverter]::ToInt32($bootBytes,18) -ne 480 -or [BitConverter]::ToInt32($bootBytes,22) -ne 272){throw 'Incomplete native frame'}
 $bootColours=@{};for($i=54;$i -lt $bootBytes.Length -and $bootColours.Count -lt 32;$i++){$bootColours[$bootBytes[$i]]=1}
 if($bootColours.Count -lt 32){throw 'Blank startup frame'}
 $bootStatus=Get-Content -LiteralPath (Join-Path $bootRun 'boot-status.txt')
 $bootLast=$bootStatus | Where-Object {$_ -match '^first frames presented free=(\d+)'}
 if(!$bootLast){throw 'Startup did not finish'}
 $bootMinimum=($bootStatus | ForEach-Object {if($_ -match 'free=(\d+)'){[int]$Matches[1]}} | Measure-Object -Minimum).Minimum
 if($bootMinimum -lt 2MB){throw "Insufficient kernel headroom: $bootMinimum"}
 if(Select-String -LiteralPath $bootLog -Pattern 'MEMBLOCK_ALLOC_FAILED|Invalid address|SCE_MP3_ERROR|CPU crash' -Quiet){throw 'Emulator memory/audio failure'}
 $bootChecks=@('PASS exact retail EBOOT renders a complete nonblank native 480x272 frame','PASS startup reaches the first presented frames','PASS startup retains at least 2 MiB of kernel headroom','PASS no memory or MP3 decoding errors')
 if($Music){
  if(!(Select-String -LiteralPath $bootLog -Pattern 'sceMp3Decode' -Quiet)){throw 'MP3 decoder did not run'}
  $bootChecks+='PASS actual supplied MP3 reaches the decoder during live rendering'
 }
 $bootChecks+="INFO model=$Model scene=$Scene minimum_free=$bootMinimum EBOOT_SHA256=$((Get-FileHash -LiteralPath (Join-Path $bootRun 'EBOOT.PBP') -Algorithm SHA256).Hash)"
 $bootChecks+='RESULT 0 failures';$bootChecks | Set-Content -LiteralPath (Join-Path $bootRun 'boot-check.txt');$bootChecks
 Write-Output "Review: $bootRun"
}finally{if($bootProcess -and !$bootProcess.HasExited){Stop-Process -Id $bootProcess.Id}}
