param([Parameter(Mandatory=$true)][string]$Folder)
$ErrorActionPreference='Stop'
$reviewDir=(Resolve-Path -LiteralPath $Folder).Path
if(!(Test-Path -LiteralPath "$reviewDir/lave-review.flag")){throw 'Use a disposable folder containing EBOOT.PBP and lave-review.flag.'}
if(!(Test-Path -LiteralPath "$reviewDir/EBOOT.PBP")){throw 'The review build is missing.'}
$emulator=(Resolve-Path -LiteralPath "$PSScriptRoot/../../../work/ppsspp/PPSSPPWindows64.exe").Path
$reviewStarted=Get-Date
$reviewProcess=Start-Process -FilePath $emulator -ArgumentList ('"'+$reviewDir+'/EBOOT.PBP"') -WorkingDirectory $reviewDir -WindowStyle Hidden -PassThru
try {
 $reviewEnd=(Get-Date).AddSeconds(55)
 while((Get-Date) -lt $reviewEnd){
  if((Test-Path -LiteralPath "$reviewDir/lave-review.txt") -and (Get-Item -LiteralPath "$reviewDir/lave-review.txt").LastWriteTime -ge $reviewStarted -and (Select-String -LiteralPath "$reviewDir/lave-review.txt" -Pattern '^RESULT ' -Quiet)){break}
  Start-Sleep -Milliseconds 250
 }
 $report=Get-Content -LiteralPath "$reviewDir/lave-review.txt"
 $report
 if(!($report -match '^RESULT 0 failures$')){throw 'Lave review failed or timed out.'}
} finally {
 if(!$reviewProcess.HasExited){Stop-Process -Id $reviewProcess.Id}
}
