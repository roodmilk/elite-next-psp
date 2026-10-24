param([string]$Eboot="$PSScriptRoot/../EBOOT.PBP",[string]$Emulator="$PSScriptRoot/../../../work/ppsspp/PPSSPPWindows64.exe")
$ErrorActionPreference='Stop'
$guildRun=Join-Path (Split-Path $PSScriptRoot -Parent) ('outputs/guild-navigation-'+(Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
New-Item -ItemType Directory -Path $guildRun | Out-Null
Copy-Item -LiteralPath $Eboot -Destination (Join-Path $guildRun 'EBOOT.PBP')
Set-Content -LiteralPath (Join-Path $guildRun 'guild-navigation-review.flag') -Value '1'
$guildProcess=Start-Process -FilePath (Resolve-Path -LiteralPath $Emulator).Path -ArgumentList ('"'+(Join-Path $guildRun 'EBOOT.PBP')+'"') -WorkingDirectory $guildRun -WindowStyle Hidden -PassThru
try {
 $guildReport=Join-Path $guildRun 'guild-navigation-review.txt';$guildDeadline=(Get-Date).AddSeconds(60)
 while((Get-Date) -lt $guildDeadline){
  if((Test-Path -LiteralPath $guildReport)-and(Select-String -LiteralPath $guildReport -Pattern '^RESULT ' -Quiet)){break}
  Start-Sleep -Milliseconds 250
 }
 Get-Content -LiteralPath $guildReport
 Write-Output "Review: $guildRun"
 if(!(Select-String -LiteralPath $guildReport -Pattern '^RESULT 0 failures$' -Quiet)){throw 'Guild navigation checks failed or timed out'}
} finally {if(!$guildProcess.HasExited){Stop-Process -Id $guildProcess.Id}}
