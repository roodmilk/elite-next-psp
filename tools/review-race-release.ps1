param([string]$Eboot="$PSScriptRoot/../EBOOT.PBP",[string]$Emulator="$PSScriptRoot/../../../work/ppsspp/PPSSPPWindows64.exe",[string]$MusicTrack='',[switch]$FocusedOnly)
$ErrorActionPreference='Stop'
$startupSuite=Join-Path (Split-Path $PSScriptRoot -Parent) ('outputs/race-release-review-'+(Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
New-Item -ItemType Directory -Path $startupSuite | Out-Null
$startupResults=@()
$missionCases=@(@('space-racing-review','space-racing-review'),@('mission-reward-review','mission-reward-review'),@('intro-film-review','intro-film-review'),@('opening-check','opening-check'),@('core-check','game-check'))
foreach($startupCase in $missionCases){
 if($FocusedOnly -and $startupCase[0] -ne 'space-racing-review'){continue}
 $startupRun=Join-Path $startupSuite $startupCase[0]
 New-Item -ItemType Directory -Path $startupRun | Out-Null
 Copy-Item -LiteralPath $Eboot -Destination (Join-Path $startupRun 'EBOOT.PBP')
 Set-Content -LiteralPath (Join-Path $startupRun ($startupCase[0]+'.flag')) -Value 1
 if($MusicTrack -and $startupCase[0] -eq 'intro-film-review'){
  if([IO.Path]::GetExtension($MusicTrack).ToLowerInvariant() -ne '.mp3'){throw 'Use a supplied MP3 for the audio coexistence review'}
  $startupMusic=Join-Path $startupRun 'music/Elite Exploration'
  New-Item -ItemType Directory -Path $startupMusic -Force | Out-Null
  Copy-Item -LiteralPath $MusicTrack -Destination (Join-Path $startupMusic 'test-track.mp3')
  Set-Content -LiteralPath (Join-Path $startupRun 'intro-film-audio.flag') -Value 1
 }
 if($MusicTrack -and $startupCase[0] -eq 'space-racing-review'){
  foreach($stationFolder in @('music/Sol Rock','music/Elite Exploration')){
   $raceMusic=Join-Path $startupRun $stationFolder
   New-Item -ItemType Directory -Path $raceMusic -Force | Out-Null
   Copy-Item -LiteralPath $MusicTrack -Destination (Join-Path $raceMusic 'test-track.mp3')
  }
  Set-Content -LiteralPath (Join-Path $startupRun 'race-audio.flag') -Value 1
 }
 $raceConfig=Join-Path $startupRun 'isolated.ini'
 $raceIni=[IO.File]::ReadAllText((Join-Path (Split-Path (Resolve-Path -LiteralPath $Emulator).Path -Parent) 'memstick/PSP/SYSTEM/ppsspp.ini'))
 $raceIni=$raceIni.Replace('IgnoreBadMemAccess = True','IgnoreBadMemAccess = False')
 [IO.File]::WriteAllText($raceConfig,$raceIni)
 $startupProcess=Start-Process -FilePath (Resolve-Path -LiteralPath $Emulator).Path -ArgumentList @('--config="'+$raceConfig+'"','--log="'+(Join-Path $startupRun 'emulator.log')+'"','"'+(Join-Path $startupRun 'EBOOT.PBP')+'"') -WorkingDirectory $startupRun -WindowStyle Hidden -PassThru
 try {
  $startupReport=Join-Path $startupRun ($startupCase[1]+'.txt');$startupDeadline=(Get-Date).AddSeconds(240)
  while((Get-Date) -lt $startupDeadline){if((Test-Path -LiteralPath $startupReport)-and(Select-String -LiteralPath $startupReport -Pattern '^RESULT ' -Quiet)){break};if($startupProcess.HasExited){break};Start-Sleep -Milliseconds 250}
  $startupPass=(Test-Path -LiteralPath $startupReport)-and(Select-String -LiteralPath $startupReport -Pattern '^RESULT 0 failures$' -Quiet)
  $startupCount=if(Test-Path -LiteralPath $startupReport){(Select-String -LiteralPath $startupReport -Pattern '^PASS ').Count}else{0}
  $startupResults+=@{test=$startupCase[0];passed=[bool]$startupPass;checks=$startupCount;report=$startupReport}
  $startupResults | ConvertTo-Json -Depth 3 | Set-Content -LiteralPath (Join-Path $startupSuite 'results.json')
  Write-Output "$($startupCase[0]): $startupCount checks, passed=$startupPass"
  if($startupCase[0] -ne 'core-check' -and !$startupPass){if(Test-Path -LiteralPath $startupReport){Get-Content $startupReport};throw 'Startup review failed or incomplete'}
 }finally{if(!$startupProcess.HasExited){Stop-Process -Id $startupProcess.Id}}
}
Write-Output "Review: $startupSuite"
