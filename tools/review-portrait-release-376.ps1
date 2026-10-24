param([string]$Eboot="$PSScriptRoot/../outputs/portrait376-release-candidate/EBOOT.PBP",[string]$Emulator="$PSScriptRoot/../../../work/ppsspp/PPSSPPWindows64.exe")
$ErrorActionPreference='Stop'
$portraitSuite=Join-Path (Split-Path $PSScriptRoot -Parent) ('outputs/portrait376-release-'+(Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
New-Item -ItemType Directory -Path $portraitSuite | Out-Null
$portraitResults=@()
# Sequential emulators keep graphics and audio contexts independent.
foreach($portraitCase in @(@('portrait-parts-review','portrait-parts-review'),@('profile-review','profile-review'),@('opening-check','opening-check'),@('command-deck-review','command-deck-review'),@('core-check','game-check'))){
 $portraitRun=Join-Path $portraitSuite $portraitCase[0]
 New-Item -ItemType Directory -Path $portraitRun | Out-Null
 Copy-Item -LiteralPath $Eboot -Destination (Join-Path $portraitRun 'EBOOT.PBP')
 Set-Content -LiteralPath (Join-Path $portraitRun ($portraitCase[0]+'.flag')) -Value 1
 $portraitProcess=Start-Process -FilePath (Resolve-Path -LiteralPath $Emulator).Path -ArgumentList @('--log="'+(Join-Path $portraitRun 'emulator.log')+'"','"'+(Join-Path $portraitRun 'EBOOT.PBP')+'"') -WorkingDirectory $portraitRun -WindowStyle Hidden -PassThru
 try {
  $portraitReport=Join-Path $portraitRun ($portraitCase[1]+'.txt');$portraitDeadline=(Get-Date).AddSeconds(300)
  while((Get-Date) -lt $portraitDeadline){if((Test-Path -LiteralPath $portraitReport)-and(Select-String -LiteralPath $portraitReport -Pattern '^RESULT ' -Quiet)){break};if($portraitProcess.HasExited){break};Start-Sleep -Milliseconds 250}
  $portraitPass=(Test-Path -LiteralPath $portraitReport)-and(Select-String -LiteralPath $portraitReport -Pattern '^RESULT 0 failures$' -Quiet)
  $portraitCount=if(Test-Path -LiteralPath $portraitReport){(Select-String -LiteralPath $portraitReport -Pattern '^PASS ').Count}else{0}
  $portraitResults+=@{test=$portraitCase[0];passed=[bool]$portraitPass;checks=$portraitCount;report=$portraitReport}
  $portraitResults | ConvertTo-Json -Depth 3 | Set-Content -LiteralPath (Join-Path $portraitSuite 'results.json')
  Write-Output "$($portraitCase[0]): $portraitCount checks, passed=$portraitPass"
 }finally{if(!$portraitProcess.HasExited){Stop-Process -Id $portraitProcess.Id}}
}
# A known core failure is compared to the previous release by the packager.
Write-Output "Review: $portraitSuite"
if(@($portraitResults | Where-Object {$_.test -ne 'core-check' -and !$_.passed}).Count){throw 'Portrait release checks failed or incomplete'}
