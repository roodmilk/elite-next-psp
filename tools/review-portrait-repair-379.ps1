param([string]$Eboot="$PSScriptRoot/../outputs/profile379-candidate/EBOOT.PBP",[string]$Emulator="$PSScriptRoot/../../../work/ppsspp/PPSSPPWindows64.exe")
$ErrorActionPreference='Stop'
$repairSuite=Join-Path (Split-Path $PSScriptRoot -Parent) ('outputs/profile379-release-'+(Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
New-Item -ItemType Directory -Path $repairSuite | Out-Null
$repairResults=@()
foreach($repairCase in @(@('portrait-parts-review','portrait-parts-review'),@('profile-review','profile-review'),@('opening-check','opening-check'),@('core-check','game-check'))){
 $repairRun=Join-Path $repairSuite $repairCase[0]
 New-Item -ItemType Directory -Path $repairRun | Out-Null
 Copy-Item -LiteralPath $Eboot -Destination (Join-Path $repairRun 'EBOOT.PBP')
 Set-Content -LiteralPath (Join-Path $repairRun ($repairCase[0]+'.flag')) -Value 1
 $repairProcess=Start-Process -FilePath (Resolve-Path -LiteralPath $Emulator).Path -ArgumentList @('--log="'+(Join-Path $repairRun 'emulator.log')+'"','"'+(Join-Path $repairRun 'EBOOT.PBP')+'"') -WorkingDirectory $repairRun -WindowStyle Hidden -PassThru
 try {
  $repairReport=Join-Path $repairRun ($repairCase[1]+'.txt');$repairDeadline=(Get-Date).AddSeconds(120)
  while((Get-Date) -lt $repairDeadline){if((Test-Path -LiteralPath $repairReport)-and(Select-String -LiteralPath $repairReport -Pattern '^RESULT ' -Quiet)){break};if($repairProcess.HasExited){break};Start-Sleep -Milliseconds 250}
  $repairPass=(Test-Path -LiteralPath $repairReport)-and(Select-String -LiteralPath $repairReport -Pattern '^RESULT 0 failures$' -Quiet)
  $repairCount=if(Test-Path -LiteralPath $repairReport){(Select-String -LiteralPath $repairReport -Pattern '^PASS ').Count}else{0}
  $repairResults+=@{test=$repairCase[0];passed=[bool]$repairPass;checks=$repairCount;report=$repairReport}
  $repairResults | ConvertTo-Json -Depth 3 | Set-Content -LiteralPath (Join-Path $repairSuite 'results.json')
  Write-Output "$($repairCase[0]): $repairCount checks, passed=$repairPass"
 }finally{if(!$repairProcess.HasExited){Stop-Process -Id $repairProcess.Id}}
}
Write-Output "Review: $repairSuite"
if(@($repairResults | Where-Object {$_.test -ne 'core-check' -and !$_.passed}).Count){throw 'Portrait repair checks failed or incomplete'}
