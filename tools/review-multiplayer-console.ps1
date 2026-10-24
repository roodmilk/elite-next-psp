param([string]$Eboot="$PSScriptRoot/../EBOOT.PBP",[ValidateSet('Console','Online','Local')][string]$Mode='Console',[string]$Emulator="$PSScriptRoot/../../../work/ppsspp/PPSSPPWindows64.exe")
$ErrorActionPreference='Stop'
$multiRun=Join-Path (Split-Path $PSScriptRoot -Parent) ('outputs/multiplayer-'+$Mode.ToLower()+'-'+(Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
New-Item -ItemType Directory -Path $multiRun | Out-Null
Copy-Item -LiteralPath $Eboot -Destination (Join-Path $multiRun 'EBOOT.PBP')
$multiName=switch($Mode){'Online'{'online-review'}'Local'{'local-wifi-review'}default{'multiplayer-console-review'}}
Set-Content -LiteralPath (Join-Path $multiRun ($multiName+'.flag')) -Value '1'
$multiProcess=Start-Process -FilePath (Resolve-Path -LiteralPath $Emulator).Path -ArgumentList ('"'+(Join-Path $multiRun 'EBOOT.PBP')+'"') -WorkingDirectory $multiRun -WindowStyle Hidden -PassThru
try {
 $multiReport=Join-Path $multiRun ($multiName+'.txt');$multiDeadline=(Get-Date).AddSeconds(360)
 while((Get-Date) -lt $multiDeadline){if((Test-Path -LiteralPath $multiReport)-and(Select-String -LiteralPath $multiReport -Pattern '^RESULT ' -Quiet)){break};Start-Sleep -Milliseconds 250}
 Get-Content -LiteralPath $multiReport
 Write-Output "Review: $multiRun"
 if(!(Select-String -LiteralPath $multiReport -Pattern '^RESULT 0 failures$' -Quiet)){throw 'Multiplayer checks failed or timed out'}
}finally{if(!$multiProcess.HasExited){Stop-Process -Id $multiProcess.Id}}
