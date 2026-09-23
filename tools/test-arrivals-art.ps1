param([Parameter(Mandatory=$true)][string]$Emulator)
$ErrorActionPreference='Stop'
$repoRoot=Split-Path $PSScriptRoot -Parent
$evidence=Join-Path $repoRoot ('../../arrivals-'+(Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
New-Item -ItemType Directory -Path $evidence | Out-Null
$evidence=(Resolve-Path $evidence).Path
Copy-Item -LiteralPath "$repoRoot/EBOOT.PBP" -Destination $evidence
New-Item -ItemType File -Path "$evidence/smoke.flag","$evidence/arrivals-art.flag" | Out-Null
$process=Start-Process -FilePath $Emulator -ArgumentList ('"'+$evidence+'/EBOOT.PBP"') -WorkingDirectory $evidence -WindowStyle Hidden -PassThru
try{
 $deadline=(Get-Date).AddSeconds(50)
 do{
  Start-Sleep -Milliseconds 250
  $done=(Test-Path "$evidence/performance-check.txt") -and (Select-String "$evidence/performance-check.txt" -Pattern '^RESULT ' -Quiet)
 }while(!$done -and (Get-Date) -lt $deadline)
 foreach($name in @('game','input','steering','radio','performance')){
  $report=Join-Path $evidence "$name-check.txt"
  if(!(Test-Path $report) -or !(Select-String $report -Pattern '^RESULT 0 failures$' -Quiet) -or (Select-String $report -Pattern '^FAIL ' -Quiet)){throw "Failed $name; evidence $evidence"}
 }
 Get-Content "$evidence/arrivals-art-check.txt"
 Write-Output "All five smoke groups pass. Evidence: $evidence"
}finally{if(!$process.HasExited){Stop-Process -Id $process.Id}}
