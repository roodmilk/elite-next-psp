param([Parameter(Mandatory=$true)][string]$Folder)
$ErrorActionPreference='Stop'
$reviewDir=(Resolve-Path -LiteralPath $Folder).Path
if(!(Test-Path -LiteralPath "$reviewDir/field-art-review.flag")){throw 'Use a disposable folder with field-art-review.flag.'}
$emulator=(Resolve-Path -LiteralPath "$PSScriptRoot/../../../work/ppsspp/PPSSPPWindows64.exe").Path
if(Get-Process PPSSPPWindows64 -ErrorAction SilentlyContinue){throw 'An emulator is already running; close it before this isolated review.'}
$reviewStarted=Get-Date
$reviewProcess=Start-Process -FilePath $emulator -ArgumentList ('"'+$reviewDir+'/EBOOT.PBP"') -WorkingDirectory $reviewDir -WindowStyle Hidden -PassThru
try {
 $reviewEnd=(Get-Date).AddSeconds(55)
 while((Get-Date) -lt $reviewEnd){
  if((Test-Path -LiteralPath "$reviewDir/field-art-review.txt") -and (Get-Item -LiteralPath "$reviewDir/field-art-review.txt").LastWriteTime -ge $reviewStarted -and (Select-String -LiteralPath "$reviewDir/field-art-review.txt" -Pattern '^RESULT ' -Quiet)){break}
  Start-Sleep -Milliseconds 250
 }
 $report=Get-Content -LiteralPath "$reviewDir/field-art-review.txt"
 $report
 if(!($report -match '^RESULT 0 failures$')){throw 'Field art review failed or timed out.'}
} finally {if(!$reviewProcess.HasExited){Stop-Process -Id $reviewProcess.Id}}
