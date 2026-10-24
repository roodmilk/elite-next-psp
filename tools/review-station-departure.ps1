param([string]$Eboot="$PSScriptRoot/../EBOOT.PBP",[string]$Emulator="$PSScriptRoot/../../../work/ppsspp/PPSSPPWindows64.exe",[switch]$FocusedOnly)
$ErrorActionPreference='Stop'
$departureSuite=Join-Path (Split-Path $PSScriptRoot -Parent) ('outputs/station-departure-review-'+(Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
New-Item -ItemType Directory -Path $departureSuite | Out-Null
$departureResults=@()
$departureCases=@(@('station-departure-review','station-departure-review'),@('load-arrival-review','load-arrival-review'),@('opening-check','opening-check'),@('core-check','game-check'))
foreach($departureCase in $departureCases){
 if($FocusedOnly -and $departureCase[0] -ne 'station-departure-review'){continue}
 $departureRun=Join-Path $departureSuite $departureCase[0]
 New-Item -ItemType Directory -Path $departureRun | Out-Null
 Copy-Item -LiteralPath $Eboot -Destination (Join-Path $departureRun 'EBOOT.PBP')
 Set-Content -LiteralPath (Join-Path $departureRun ($departureCase[0]+'.flag')) -Value 1
 $departureProcess=Start-Process -FilePath (Resolve-Path -LiteralPath $Emulator).Path -ArgumentList @('--log="'+(Join-Path $departureRun 'emulator.log')+'"','"'+(Join-Path $departureRun 'EBOOT.PBP')+'"') -WorkingDirectory $departureRun -WindowStyle Hidden -PassThru
 try {
  $departureReport=Join-Path $departureRun ($departureCase[1]+'.txt');$departureDeadline=(Get-Date).AddSeconds(150)
  while((Get-Date) -lt $departureDeadline){if((Test-Path -LiteralPath $departureReport)-and(Select-String -LiteralPath $departureReport -Pattern '^RESULT ' -Quiet)){break};if($departureProcess.HasExited){break};Start-Sleep -Milliseconds 250}
  $departurePass=(Test-Path -LiteralPath $departureReport)-and(Select-String -LiteralPath $departureReport -Pattern '^RESULT 0 failures$' -Quiet)
  $departureCount=if(Test-Path -LiteralPath $departureReport){(Select-String -LiteralPath $departureReport -Pattern '^PASS ').Count}else{0}
  $departureResults+=@{test=$departureCase[0];passed=[bool]$departurePass;checks=$departureCount;report=$departureReport}
  $departureResults | ConvertTo-Json -Depth 3 | Set-Content -LiteralPath (Join-Path $departureSuite 'results.json')
  Write-Output "$($departureCase[0]): $departureCount checks, passed=$departurePass"
 }finally{if(!$departureProcess.HasExited){Stop-Process -Id $departureProcess.Id}}
 if($departureCase[0] -eq 'station-departure-review' -and !$departurePass){Get-Content $departureReport;throw 'Departure integration review failed'}
}
Write-Output "Review: $departureSuite"
if(@($departureResults | Where-Object {$_.test -ne 'core-check' -and !$_.passed}).Count){throw 'Departure/load/opening checks failed or incomplete'}
