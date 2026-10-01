param([Parameter(Mandatory=$true)][string]$Folder)
$ErrorActionPreference='Stop'
$reviewDir=(Resolve-Path -LiteralPath $Folder).Path
if(!(Test-Path -LiteralPath "$reviewDir/fauna-review.flag")){throw 'Use a disposable fauna-review folder.'}
$emulator=(Resolve-Path -LiteralPath "$PSScriptRoot/../../../work/ppsspp/PPSSPPWindows64.exe").Path
if(Get-Process PPSSPPWindows64 -ErrorAction SilentlyContinue){throw 'An emulator is already running.'}
$reviewStarted=Get-Date
$reviewProcess=Start-Process -FilePath $emulator -ArgumentList ('"'+$reviewDir+'/EBOOT.PBP"') -WorkingDirectory $reviewDir -WindowStyle Hidden -PassThru
try {
 $reviewEnd=(Get-Date).AddSeconds(55)
 while((Get-Date) -lt $reviewEnd){
  if((Test-Path -LiteralPath "$reviewDir/fauna-review.txt") -and (Get-Item -LiteralPath "$reviewDir/fauna-review.txt").LastWriteTime -ge $reviewStarted -and (Select-String -LiteralPath "$reviewDir/fauna-review.txt" -Pattern '^RESULT ' -Quiet)){break}
  Start-Sleep -Milliseconds 250
 }
 Get-Content -LiteralPath "$reviewDir/fauna-behaviour.txt"
 $report=Get-Content -LiteralPath "$reviewDir/fauna-review.txt"
 $report
 if((Get-Item -LiteralPath "$reviewDir/fauna-review.txt").LastWriteTime -lt $reviewStarted -or !($report -match '^RESULT 0 failures$')){throw 'Fauna review failed or timed out.'}
} finally {if(!$reviewProcess.HasExited){Stop-Process -Id $reviewProcess.Id}}
