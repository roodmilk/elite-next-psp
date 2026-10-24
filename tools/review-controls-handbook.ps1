param(
 [string]$Eboot="$PSScriptRoot/../outputs/controls-handbook-candidate/EBOOT.PBP",
 [string]$Emulator="$PSScriptRoot/../../../work/ppsspp/PPSSPPWindows64.exe",
 [switch]$Regression,
 [switch]$Core
)
$ErrorActionPreference='Stop'
$handbookRun=Join-Path (Split-Path $PSScriptRoot -Parent) ('outputs/controls-review-'+(Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
New-Item -ItemType Directory -Path $handbookRun | Out-Null
Copy-Item -LiteralPath $Eboot -Destination (Join-Path $handbookRun 'EBOOT.PBP')
$handbookFlag=if($Core){'core-check.flag'}elseif($Regression){'smoke.flag'}else{'controls-handbook-review.flag'}
Set-Content -LiteralPath (Join-Path $handbookRun $handbookFlag) -Value '1'
$handbookProcess=Start-Process -FilePath (Resolve-Path -LiteralPath $Emulator).Path -ArgumentList ('"'+(Join-Path $handbookRun 'EBOOT.PBP')+'"') -WorkingDirectory $handbookRun -WindowStyle Hidden -PassThru
try {
 $handbookReport=Join-Path $handbookRun $(if($Core){'game-check.txt'}elseif($Regression){'performance-check.txt'}else{'controls-handbook-review.txt'})
 $handbookDeadline=(Get-Date).AddSeconds(360)
 while((Get-Date) -lt $handbookDeadline){
  if((Test-Path -LiteralPath $handbookReport)-and(Select-String -LiteralPath $handbookReport -Pattern '^RESULT ' -Quiet)){break}
  Start-Sleep -Milliseconds 250
 }
 if($Regression -or $Core){
  $handbookGroups=if($Core){@('game','steering','radio')}else{@('game','input','steering','radio','performance')}
  foreach($group in $handbookGroups){
   $handbookGroup=Join-Path $handbookRun ($group+'-check.txt')
   if(!(Test-Path -LiteralPath $handbookGroup)){throw "Missing report: $handbookGroup"}
   Write-Output "$group`:"
   Select-String -LiteralPath $handbookGroup -Pattern '^(FAIL|RESULT) ' | ForEach-Object {$_.Line}
  }
 }else{
  Get-Content -LiteralPath $handbookReport
  if(!(Select-String -LiteralPath $handbookReport -Pattern '^RESULT 0 failures$' -Quiet)){throw 'Controls handbook checks failed or timed out'}
 }
 Write-Output "Review: $handbookRun"
}finally{if(!$handbookProcess.HasExited){Stop-Process -Id $handbookProcess.Id}}
