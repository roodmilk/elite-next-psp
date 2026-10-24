param([string]$Eboot="$PSScriptRoot/../EBOOT.PBP",[string]$Emulator="$PSScriptRoot/../../../work/ppsspp/PPSSPPWindows64.exe")
$ErrorActionPreference='Stop'
$arrivalSuite=Join-Path (Split-Path $PSScriptRoot -Parent) ('outputs/load-arrival-review-'+(Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
New-Item -ItemType Directory -Path $arrivalSuite | Out-Null
$arrivalResults=@()
foreach($arrivalCase in @(@('load-arrival-review','load-arrival-review'),@('intro-film-review','intro-film-review'),@('command-deck-review','command-deck-review'),@('menu-preview-check','menu-preview-check'),@('opening-check','opening-check'),@('core-check','game-check'))){
 $arrivalRun=Join-Path $arrivalSuite $arrivalCase[0]
 New-Item -ItemType Directory -Path $arrivalRun | Out-Null
 Copy-Item -LiteralPath $Eboot -Destination (Join-Path $arrivalRun 'EBOOT.PBP')
 Set-Content -LiteralPath (Join-Path $arrivalRun ($arrivalCase[0]+'.flag')) -Value 1
 $arrivalProcess=Start-Process -FilePath (Resolve-Path -LiteralPath $Emulator).Path -ArgumentList @('--log="'+(Join-Path $arrivalRun 'emulator.log')+'"','"'+(Join-Path $arrivalRun 'EBOOT.PBP')+'"') -WorkingDirectory $arrivalRun -WindowStyle Hidden -PassThru
 try {
  $arrivalReport=Join-Path $arrivalRun ($arrivalCase[1]+'.txt');$arrivalDeadline=(Get-Date).AddSeconds(150)
  while((Get-Date) -lt $arrivalDeadline){if((Test-Path -LiteralPath $arrivalReport)-and(Select-String -LiteralPath $arrivalReport -Pattern '^RESULT ' -Quiet)){break};if($arrivalProcess.HasExited){break};Start-Sleep -Milliseconds 250}
  $arrivalPass=(Test-Path -LiteralPath $arrivalReport)-and(Select-String -LiteralPath $arrivalReport -Pattern '^RESULT 0 failures$' -Quiet)
  $arrivalCount=if(Test-Path -LiteralPath $arrivalReport){(Select-String -LiteralPath $arrivalReport -Pattern '^PASS ').Count}else{0}
  $arrivalResults+=@{test=$arrivalCase[0];passed=[bool]$arrivalPass;checks=$arrivalCount;report=$arrivalReport}
  $arrivalResults | ConvertTo-Json -Depth 3 | Set-Content -LiteralPath (Join-Path $arrivalSuite 'results.json')
  Write-Output "$($arrivalCase[0]): $arrivalCount checks, passed=$arrivalPass"
 }finally{if(!$arrivalProcess.HasExited){Stop-Process -Id $arrivalProcess.Id}}
 if($arrivalCase[0] -eq 'load-arrival-review' -and !$arrivalPass){Get-Content $arrivalReport;throw 'Station arrival review failed'}
}
Write-Output "Review: $arrivalSuite"
if(@($arrivalResults | Where-Object {$_.test -ne 'core-check' -and !$_.passed}).Count){throw 'Load/intro/deck/opening checks failed or incomplete'}
