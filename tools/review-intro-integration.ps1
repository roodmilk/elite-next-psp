param([string]$Eboot="$PSScriptRoot/../EBOOT.PBP",[string]$Emulator="$PSScriptRoot/../../../work/ppsspp/PPSSPPWindows64.exe")
$ErrorActionPreference='Stop'
$introSuite=Join-Path (Split-Path $PSScriptRoot -Parent) ('outputs/intro-integration-'+(Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
New-Item -ItemType Directory -Path $introSuite | Out-Null
$introResults=@()
# Separate, sequential emulator processes avoid graphics-backend contention.
foreach($introCase in @(@('opening-check','opening-check'),@('multiplayer-console-review','multiplayer-console-review'),@('portrait-parts-review','portrait-parts-review'),@('command-deck-review','command-deck-review'))){
 $introRun=Join-Path $introSuite $introCase[0]
 New-Item -ItemType Directory -Path $introRun | Out-Null
 Copy-Item -LiteralPath $Eboot -Destination (Join-Path $introRun 'EBOOT.PBP')
 Set-Content -LiteralPath (Join-Path $introRun ($introCase[0]+'.flag')) -Value 1
 $introProcess=Start-Process -FilePath (Resolve-Path -LiteralPath $Emulator).Path -ArgumentList @('--log="'+(Join-Path $introRun 'emulator.log')+'"','"'+(Join-Path $introRun 'EBOOT.PBP')+'"') -WorkingDirectory $introRun -WindowStyle Hidden -PassThru
 try {
  $introReport=Join-Path $introRun ($introCase[1]+'.txt');$introDeadline=(Get-Date).AddSeconds(300)
  while((Get-Date) -lt $introDeadline){if((Test-Path -LiteralPath $introReport)-and(Select-String -LiteralPath $introReport -Pattern '^RESULT ' -Quiet)){break};if($introProcess.HasExited){break};Start-Sleep -Milliseconds 250}
  $introPass=(Test-Path -LiteralPath $introReport)-and(Select-String -LiteralPath $introReport -Pattern '^RESULT 0 failures$' -Quiet)
  $introCount=if(Test-Path -LiteralPath $introReport){(Select-String -LiteralPath $introReport -Pattern '^PASS ').Count}else{0}
  $introResults+=@{test=$introCase[0];passed=[bool]$introPass;checks=$introCount;report=$introReport}
  $introResults | ConvertTo-Json -Depth 3 | Set-Content -LiteralPath (Join-Path $introSuite 'results.json')
  Write-Output "$($introCase[0]): $introCount checks, passed=$introPass"
 }finally{if(!$introProcess.HasExited){Stop-Process -Id $introProcess.Id}}
}
Write-Output "Review: $introSuite"
if(@($introResults | Where-Object {!$_.passed}).Count){throw 'Intro integration suite has failed or incomplete reports'}
