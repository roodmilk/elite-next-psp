param([Parameter(Mandatory=$true)][string]$Folder)
$ErrorActionPreference='Stop'
$reviewDir=(Resolve-Path -LiteralPath $Folder).Path
if(!(Test-Path -LiteralPath "$reviewDir/lave-world-review.flag")){throw 'Use a disposable review directory.'}
if(Test-Path -LiteralPath "$reviewDir/lave-world-review.txt"){throw 'Use a fresh review directory: an existing buffered report can appear fresh before its contents have been replaced.'}
$emulator=(Resolve-Path -LiteralPath "$PSScriptRoot/../../../work/ppsspp/PPSSPPWindows64.exe").Path
if(Get-Process PPSSPPWindows64 -ErrorAction SilentlyContinue){throw 'An emulator is already running.'}
$started=Get-Date
$process=Start-Process -FilePath $emulator -ArgumentList ('"'+$reviewDir+'/EBOOT.PBP"') -WorkingDirectory $reviewDir -WindowStyle Hidden -PassThru
try {
 $deadline=(Get-Date).AddSeconds(180)
 while((Get-Date) -lt $deadline){
  if((Test-Path "$reviewDir/lave-world-review.txt") -and (Get-Item "$reviewDir/lave-world-review.txt").LastWriteTime -ge $started -and (Select-String "$reviewDir/lave-world-review.txt" -Pattern '^RESULT ' -Quiet)){break}
  Start-Sleep -Milliseconds 250
 }
 Get-Content "$reviewDir/lave-world-tests.txt"
 $report=Get-Content "$reviewDir/lave-world-review.txt";$report
 if((Get-Item "$reviewDir/lave-world-review.txt").LastWriteTime -lt $started -or !($report -match '^RESULT 0 failures$')){throw 'World review failed or timed out.'}
}finally{if(!$process.HasExited){Stop-Process -Id $process.Id}}
