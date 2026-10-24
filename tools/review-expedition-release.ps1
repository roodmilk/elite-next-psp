param([string]$Eboot="$PSScriptRoot/../outputs/mission388-fast/EBOOT.PBP",[string]$RaceTrack="$PSScriptRoot/../outputs/Secret Agent Rock-test.mp3",[string]$IntroTrack='', [switch]$Full, [switch]$InputOnly)
$ErrorActionPreference='Stop'
$reviewRoot=Join-Path (Split-Path $PSScriptRoot -Parent) ('outputs/expedition-review-'+(Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
New-Item -ItemType Directory -Path $reviewRoot | Out-Null
$reviewCases=@(,@('mission-expansion-review','mission-expansion-review'))
if($Full){$reviewCases+=@(@('space-racing-review','space-racing-review'),@('mission-reward-review','mission-reward-review'),@('intro-film-review','intro-film-review'),@('opening-check','opening-check'),@('core-check','game-check'),@('profile-law-review','input-check'))}
if($InputOnly){$reviewCases=@(,@('profile-law-review','input-check'))}
$reviewResults=@()
foreach($reviewCase in $reviewCases){
 $reviewRun=Join-Path $reviewRoot $reviewCase[0];New-Item -ItemType Directory -Path $reviewRun | Out-Null
 Copy-Item -LiteralPath $Eboot -Destination (Join-Path $reviewRun 'EBOOT.PBP');Set-Content -LiteralPath (Join-Path $reviewRun ($reviewCase[0]+'.flag')) -Value 1
 if($reviewCase[0] -eq 'space-racing-review'){
  foreach($station in @('Sol Rock','Elite Exploration')){New-Item -ItemType Directory -Path (Join-Path $reviewRun "music/$station") -Force | Out-Null;Copy-Item -LiteralPath $RaceTrack -Destination (Join-Path $reviewRun "music/$station/Secret Agent Rock.mp3")}
  Set-Content -LiteralPath (Join-Path $reviewRun 'race-audio.flag') -Value 1
 }
 if($reviewCase[0] -eq 'intro-film-review' -and $IntroTrack){New-Item -ItemType Directory -Path (Join-Path $reviewRun 'music/Elite Exploration') -Force | Out-Null;Copy-Item -LiteralPath $IntroTrack -Destination (Join-Path $reviewRun 'music/Elite Exploration/intro-test.mp3');Set-Content -LiteralPath (Join-Path $reviewRun 'intro-film-audio.flag') -Value 1}
 $reviewEmulator=(Resolve-Path "$PSScriptRoot/../../../work/ppsspp/PPSSPPWindows64.exe").Path
 $reviewProcess=Start-Process -FilePath $reviewEmulator -ArgumentList @('--log="'+(Join-Path $reviewRun 'emulator.log')+'"','"'+(Join-Path $reviewRun 'EBOOT.PBP')+'"') -WorkingDirectory $reviewRun -WindowStyle Hidden -PassThru
 try{
  $reviewReport=Join-Path $reviewRun ($reviewCase[1]+'.txt');$reviewDeadline=(Get-Date).AddSeconds(240)
  while((Get-Date) -lt $reviewDeadline){if((Test-Path $reviewReport)-and(Select-String -LiteralPath $reviewReport -Pattern '^RESULT ' -Quiet)){break};if($reviewProcess.HasExited){break};Start-Sleep -Milliseconds 250}
  $reviewPass=(Test-Path $reviewReport)-and(Select-String -LiteralPath $reviewReport -Pattern '^RESULT 0 failures$' -Quiet)
  $reviewCount=if(Test-Path $reviewReport){(Select-String -LiteralPath $reviewReport -Pattern '^PASS ').Count}else{0}
  $reviewResults+=@{test=$reviewCase[0];passed=[bool]$reviewPass;checks=$reviewCount;report=$reviewReport};$reviewResults|ConvertTo-Json -Depth 4 | Set-Content (Join-Path $reviewRoot 'results.json')
  Write-Output "$($reviewCase[0]): $reviewCount checks; passed=$reviewPass"
  if(!$reviewPass -and (Test-Path $reviewReport)){Select-String -LiteralPath $reviewReport -Pattern '^FAIL|^RESULT';$backend=Join-Path $reviewRun 'mission-expansion-backend.txt';if(Test-Path $backend){Select-String -LiteralPath $backend -Pattern '^FAIL|^RESULT'}}
 }finally{if(!$reviewProcess.HasExited){Stop-Process -Id $reviewProcess.Id}}
}
Write-Output "Review: $reviewRoot"

