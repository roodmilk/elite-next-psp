[CmdletBinding()]
param(
 [Parameter(Mandatory=$true)][string]$Flag,
 [ValidateRange(10,300)][int]$Seconds=60,
 [string]$Eboot="$PSScriptRoot/../outputs/magnetic-slice/review-v39-fixed/EBOOT.PBP",
 [ValidateSet(0,1)][int]$Model=1,
 [ValidatePattern('^[A-Za-z0-9][A-Za-z0-9_-]{0,31}$')][string]$RunTag='v39-review',
 [string]$FlagValue='1',
 [switch]$Retail,
 [switch]$Boot,
 [switch]$CockpitFootage,
 [switch]$CockpitMounts,
 [switch]$Smoke,
 [ValidateRange(0,4)][int]$Charm=0,
 [ValidateRange(0,16)][int]$Dashboard=0,
 [ValidateRange(0,3)][int]$PortKind=1,
 [ValidateRange(-1,22)][int]$CockpitShip=-1
)
$ErrorActionPreference='Stop'
if($Retail -and $Flag -in @('final-menu','erl-quick-race-review','cockpit-hud-review')){throw 'Diagnostic-only review flags require the diagnostic build/profile; they are not retail performance tests.'}
$candidateRoot=(Resolve-Path (Join-Path $PSScriptRoot '../../erl-implementation-candidate')).Path
if(@(Get-Process -Name 'PPSSPPWindows64','PPSSPPWindows' -ErrorAction SilentlyContinue).Count){throw 'An emulator is already running.'}
$run=Join-Path $candidateRoot "outputs/magnetic-slice/regression-$RunTag-diagnostic-$Flag"
if(Test-Path -LiteralPath $run){throw "Existing evidence directory: $run"}
$private=Join-Path $run 'emulator'
$system=Join-Path $private 'memstick/PSP/SYSTEM'
New-Item -ItemType Directory -Path $system | Out-Null
Copy-Item -LiteralPath (Resolve-Path -LiteralPath $Eboot).Path -Destination (Join-Path $run 'EBOOT.PBP')
New-Item -ItemType Directory -Force (Join-Path $run 'music') | Out-Null
Copy-Item -LiteralPath (Join-Path $candidateRoot 'music/Dance') -Destination (Join-Path $run 'music/Elite Exploration') -Recurse
Copy-Item -LiteralPath (Join-Path $candidateRoot 'emulator/PPSSPPWindows64.exe') -Destination $private
Copy-Item -LiteralPath (Join-Path $candidateRoot 'emulator/assets') -Destination $private -Recurse
Get-ChildItem -LiteralPath (Join-Path $candidateRoot 'emulator') -File -Filter '*.dll' | ForEach-Object {Copy-Item -LiteralPath $_.FullName -Destination $private}
$controls=Join-Path $system 'controls.ini'
Copy-Item -LiteralPath (Join-Path $candidateRoot 'emulator/memstick/PSP/SYSTEM/controls.ini') -Destination $controls
$ini=[IO.File]::ReadAllText((Join-Path $candidateRoot 'evidence/baseline/isolated.ini'))
$ini=[regex]::Replace($ini,'PSPModel = \d',"PSPModel = $Model")
$ini=[regex]::Replace($ini,'(?m)^(DumpFrames|DumpVideoOutput|DumpAudio)\s*=.*$','$1 = False')
$ini=[regex]::Replace($ini,'(?m)^(GameVolume|UIVolume|AchievementVolume|GamePreviewVolume)\s*=.*$','$1 = 0')
$ini=[regex]::Replace($ini,'(?m)^UISound\s*=.*$','UISound = False')
$ini=[regex]::Replace($ini,'(?m)^InternalResolution\s*=.*$','InternalResolution = 1')
$config=Join-Path $run 'isolated.ini'
[IO.File]::WriteAllText($config,$ini,[Text.UTF8Encoding]::new($false))
Set-Content -LiteralPath (Join-Path $run "$Flag.flag") -Value $FlagValue
if($Flag -eq 'encounter-flight-capture' -and $FlagValue -eq '44'){Set-Content -LiteralPath (Join-Path $run 'port-capture-kind.flag') -Value $PortKind}
if($Dashboard){Set-Content -LiteralPath (Join-Path $run 'dashboard-capture.flag') -Value $Dashboard}
if($Charm){Set-Content -LiteralPath (Join-Path $run 'charm-capture.flag') -Value $Charm}
if($Boot){Set-Content -LiteralPath (Join-Path $run 'dump-native.flag') -Value 1}
if($CockpitFootage){Set-Content -LiteralPath (Join-Path $run 'cockpit-footage.flag') -Value 1}
if($CockpitMounts){Set-Content -LiteralPath (Join-Path $run 'cockpit-mounts.flag') -Value 1}
if($CockpitShip -ge 0){Set-Content -LiteralPath (Join-Path $run 'cockpit-ship.flag') -Value $CockpitShip}
if($Smoke){Set-Content -LiteralPath (Join-Path $run 'smoke.flag') -Value 1}
$protectedBefore=@()
if($Flag -in @('erl-quick-race-review','erl-neon-review','cockpit-hud-review','charm-review','dashboard-review','loadout-picker-review','departure-hud-review','port-ad-review')){
 foreach($base in @('commander.sav','commander-2.sav','commander-3.sav','tutorial.sav','ship-paint.cfg')){
  foreach($suffix in @('','.bak','.tmp')){
   $name=$base+$suffix;$path=Join-Path $run $name
   [IO.File]::WriteAllText($path,"ISOLATED QUICK-RACE SAVE SENTINEL: $name",[Text.UTF8Encoding]::new($false))
   $protectedBefore+= [ordered]@{file=$name;sha256=(Get-FileHash -LiteralPath $path).Hash}
  }
 }
}
$report=Join-Path $run "$Flag.txt"
$legacyFixture=Join-Path $candidateRoot 'outputs/magnetic-slice/regression-v56-final-diagnostic-erl-neon-review/neon54-pickup.sav'
if(Test-Path -LiteralPath $legacyFixture){Copy-Item -LiteralPath $legacyFixture -Destination (Join-Path $run 'legacy-v37.sav')}
$v38Fixture=Join-Path $candidateRoot 'outputs/magnetic-slice/regression-v58-final-diagnostic-erl-neon-review/field15.sav'
if(Test-Path -LiteralPath $v38Fixture){Copy-Item -LiteralPath $v38Fixture -Destination (Join-Path $run 'legacy-v38.sav')}
$process=Start-Process -FilePath (Join-Path $private 'PPSSPPWindows64.exe') -ArgumentList @('--config="'+$config+'"','--controlconfig="'+$controls+'"','--log="'+(Join-Path $run 'emulator.log')+'"','"'+(Join-Path $run 'EBOOT.PBP')+'"') -WorkingDirectory $run -WindowStyle Hidden -PassThru
try {
 $deadline=(Get-Date).AddSeconds($Seconds)
 while((Get-Date)-lt$deadline){
  if($Boot){
   $bootFrame=Get-Item -LiteralPath (Join-Path $run 'native-480x272.bmp') -ErrorAction SilentlyContinue
   if($bootFrame -and $bootFrame.Length -eq 391734){
    $bootBytes=[IO.File]::ReadAllBytes($bootFrame.FullName)
    if([BitConverter]::ToInt32($bootBytes,18) -ne 480 -or [BitConverter]::ToInt32($bootBytes,22) -ne 272){throw 'Invalid native boot dimensions'}
    $bootDistinct=@{};for($bootIndex=54;$bootIndex -lt $bootBytes.Length -and $bootDistinct.Count -lt 32;$bootIndex++){$bootDistinct[$bootBytes[$bootIndex]]=1}
    if($bootDistinct.Count -lt 32){throw 'Blank native boot frame'}
    if(Select-String -LiteralPath (Join-Path $run 'emulator.log') -Pattern 'MEMBLOCK_ALLOC_FAILED' -Quiet){throw 'Boot heap allocation failure'}
    @('PASS normal startup produces a complete nonblank native 480x272 frame','PASS no startup heap allocation failure','RESULT 0 failures') | Set-Content -LiteralPath $report
   }
  }
  if((Test-Path -LiteralPath $report)-and(Select-String -LiteralPath $report -Pattern '^RESULT ' -Quiet)){break}
  if($process.HasExited){break}
  Start-Sleep -Milliseconds 250
 }
 if(!(Test-Path -LiteralPath $report)-or!(Select-String -LiteralPath $report -Pattern '^RESULT ' -Quiet)){throw "Incomplete review: $report"}
 Get-Content -LiteralPath $report | Select-Object -Last 8
 $bindingProfile=if($Retail){'Retail PSP-3000 build; normal game loop, mixer and display; physical hardware confirmation pending'}else{'Diagnostic build; injected workload/logic only, no retail FPS claim'}
 $bindingScope=if($Boot){'Isolated normal retail startup, nonblank native frame; no human saves or settings modified.'}elseif($Retail){'Isolated retail orbit capture; no human saves or music/settings modified.'}else{'Injected regression fixture, not a human tutorial or career route.'}
 $binding=[ordered]@{cockpit_ship=$CockpitShip;port_kind=$PortKind;dashboard=$Dashboard;charm=$Charm;flag=$Flag;eboot_sha256=(Get-FileHash -LiteralPath (Join-Path $run 'EBOOT.PBP') -Algorithm SHA256).Hash;model=$Model;internal_resolution=1;footage=[bool]$CockpitFootage;private_music='Existing Dance tracks copied to the selected Elite Exploration station directory; source library unchanged';profile=$bindingProfile;emulator='PPSSPP v1.20.4';report="$Flag.txt";scope=$bindingScope}
 $binding | ConvertTo-Json | Set-Content -LiteralPath (Join-Path $run 'binding.json') -Encoding UTF8
 if($protectedBefore.Count){
  $protectedAfter=@($protectedBefore | ForEach-Object { [ordered]@{file=$_.file;before_sha256=$_.sha256;after_sha256=(Get-FileHash -LiteralPath (Join-Path $run $_.file)).Hash} })
  $protectedOk=@($protectedAfter | Where-Object {$_.before_sha256 -ne $_.after_sha256}).Count -eq 0
  [ordered]@{all_unchanged=$protectedOk;scope='Private sentinel saves/paint and backups/temp files; no human saves used.';files=$protectedAfter} | ConvertTo-Json -Depth 5 | Set-Content -LiteralPath (Join-Path $run 'save-isolation.json') -Encoding UTF8
  if(!$protectedOk){throw 'Quick race modified a protected file'}
 }
 if(!(Select-String -LiteralPath $report -Pattern '^RESULT 0 failures' -Quiet)){throw "Regression failed: $report"}
}finally{if(!$process.HasExited){Stop-Process -Id $process.Id;$process.WaitForExit(5000) | Out-Null}}

