param([string]$Eboot="$PSScriptRoot/../EBOOT.PBP",[string]$Emulator="$PSScriptRoot/../../../work/ppsspp/PPSSPPWindows64.exe",[string]$MusicTrack='')
$ErrorActionPreference='Stop'
$introRun=Join-Path (Split-Path $PSScriptRoot -Parent) ('outputs/intro-film-review-'+(Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
New-Item -ItemType Directory -Path $introRun | Out-Null
Copy-Item -LiteralPath $Eboot -Destination (Join-Path $introRun 'EBOOT.PBP')
Set-Content -LiteralPath (Join-Path $introRun 'intro-film-review.flag') -Value 1
if($MusicTrack){
 if([IO.Path]::GetExtension($MusicTrack).ToLowerInvariant() -ne '.mp3'){throw 'The coexistence fixture needs an MP3'}
 $introMusic=Join-Path $introRun 'music/Elite Exploration'
 New-Item -ItemType Directory -Path $introMusic -Force | Out-Null
 Copy-Item -LiteralPath $MusicTrack -Destination (Join-Path $introMusic 'test-track.mp3')
 Set-Content -LiteralPath (Join-Path $introRun 'intro-film-audio.flag') -Value 1
}
$introProcess=Start-Process -FilePath (Resolve-Path -LiteralPath $Emulator).Path -ArgumentList @('--log="'+(Join-Path $introRun 'emulator.log')+'"','"'+(Join-Path $introRun 'EBOOT.PBP')+'"') -WorkingDirectory $introRun -WindowStyle Hidden -PassThru
try {
 $introReport=Join-Path $introRun 'intro-film-review.txt';$introDeadline=(Get-Date).AddSeconds(120)
 while((Get-Date) -lt $introDeadline){if((Test-Path -LiteralPath $introReport)-and(Select-String -LiteralPath $introReport -Pattern '^RESULT ' -Quiet)){break};if($introProcess.HasExited){throw 'Intro review exited early'};Start-Sleep -Milliseconds 250}
 Get-Content -LiteralPath $introReport
 Write-Output "Review: $introRun"
 if(!(Select-String -LiteralPath $introReport -Pattern '^RESULT 0 failures$' -Quiet)){throw 'Intro film checks failed or timed out'}
} finally {if(!$introProcess.HasExited){Stop-Process -Id $introProcess.Id}}
