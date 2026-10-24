param([string]$Candidate="$PSScriptRoot/../EBOOT.PBP",[string]$Baseline="$PSScriptRoot/../dist/ELITE-NEXT-PSP/EBOOT.PBP",[string]$Emulator="$PSScriptRoot/../../../work/ppsspp/PPSSPPWindows64.exe")
$ErrorActionPreference='Stop'
$newsIsolation=Join-Path (Split-Path $PSScriptRoot -Parent) ('outputs/newspapers-isolation-'+(Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
New-Item -ItemType Directory -Path $newsIsolation | Out-Null
$newsImages=@('market-neon-0.bmp','market-neon-1.bmp','market-neon-2.bmp','market-neon-3.bmp','market-offline.bmp')
foreach($newsCase in @(@('baseline',$Baseline),@('candidate',$Candidate))){
 $newsFolder=Join-Path $newsIsolation $newsCase[0]
 New-Item -ItemType Directory -Path $newsFolder | Out-Null
 Copy-Item -LiteralPath $newsCase[1] -Destination (Join-Path $newsFolder 'EBOOT.PBP')
 Set-Content -LiteralPath (Join-Path $newsFolder 'galnet-market-capture.flag') -Value '1'
 $newsProcess=Start-Process -FilePath (Resolve-Path -LiteralPath $Emulator).Path -ArgumentList ('"'+(Join-Path $newsFolder 'EBOOT.PBP')+'"') -WorkingDirectory $newsFolder -WindowStyle Hidden -PassThru
 try{
  $newsDeadline=(Get-Date).AddSeconds(60)
  while((Get-Date) -lt $newsDeadline -and !(Test-Path -LiteralPath (Join-Path $newsFolder $newsImages[4]))){Start-Sleep -Milliseconds 250}
  foreach($newsImage in $newsImages){if(!(Test-Path -LiteralPath (Join-Path $newsFolder $newsImage))){throw "Missing $newsImage in $newsFolder"}}
 }finally{if(!$newsProcess.HasExited){Stop-Process -Id $newsProcess.Id}}
}
$newsFailures=0
foreach($newsImage in $newsImages){
 $newsOld=(Get-FileHash -LiteralPath (Join-Path $newsIsolation ('baseline/'+$newsImage))).Hash
 $newsNew=(Get-FileHash -LiteralPath (Join-Path $newsIsolation ('candidate/'+$newsImage))).Hash
 if($newsOld -ne $newsNew){Write-Output "FAIL Market changed: $newsImage";$newsFailures++}else{Write-Output "PASS Market unchanged: $newsImage"}
}
Write-Output "RESULT $newsFailures failures"
Write-Output "Review: $newsIsolation"
if($newsFailures){throw 'Market isolation regression'}
