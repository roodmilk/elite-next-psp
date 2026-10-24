param([string]$Eboot="$PSScriptRoot/../EBOOT.PBP",[string]$Emulator="$PSScriptRoot/../../../work/ppsspp/PPSSPPWindows64.exe",[string]$MusicTrack='')
$ErrorActionPreference='Stop'
$startupSuite=Join-Path (Split-Path $PSScriptRoot -Parent) ('outputs/startup-review-'+(Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
New-Item -ItemType Directory -Path $startupSuite | Out-Null
$startupResults=@()
foreach($startupCase in @(@('intro-film-review','intro-film-review'),@('opening-check','opening-check'),@('core-check','game-check'))){
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
 $startupProcess=Start-Process -FilePath (Resolve-Path -LiteralPath $Emulator).Path -ArgumentList @('--log="'+(Join-Path $startupRun 'emulator.log')+'"','"'+(Join-Path $startupRun 'EBOOT.PBP')+'"') -WorkingDirectory $startupRun -WindowStyle Hidden -PassThru
 try {
  $startupReport=Join-Path $startupRun ($startupCase[1]+'.txt');$startupDeadline=(Get-Date).AddSeconds(150)
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
