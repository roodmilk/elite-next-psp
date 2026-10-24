param([string]$Eboot="$PSScriptRoot/../EBOOT.PBP",[string]$Emulator="$PSScriptRoot/../../../work/ppsspp/PPSSPPWindows64.exe",[string]$Node='C:/Users/skarm/.cache/codex-runtimes/codex-primary-runtime/dependencies/node/bin/node.exe')
$ErrorActionPreference='Stop'
$liveRoot=Split-Path $PSScriptRoot -Parent
$liveRun=Join-Path $liveRoot ('outputs/multiplayer-live-'+(Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
$liveRuntime=Join-Path $liveRun 'emulator'
$liveSystem=Join-Path $liveRuntime 'memstick/PSP/SYSTEM'
New-Item -ItemType Directory -Path $liveSystem -Force | Out-Null
$liveEmulator=(Resolve-Path -LiteralPath $Emulator).Path
$liveEmulatorRoot=Split-Path $liveEmulator -Parent
Copy-Item -LiteralPath $liveEmulator -Destination (Join-Path $liveRuntime 'PPSSPPWindows64.exe')
Copy-Item -LiteralPath (Join-Path $liveEmulatorRoot 'assets') -Destination (Join-Path $liveRuntime 'assets') -Recurse
$liveIni=Get-Content -LiteralPath (Join-Path $liveEmulatorRoot 'memstick/PSP/SYSTEM/ppsspp.ini') -Raw
$liveIni=$liveIni.Replace('EnableWlan = False','EnableWlan = True').Replace('proAdhocServer = socom.cc','proAdhocServer = 127.0.0.1')
$liveIni=[regex]::Replace($liveIni,'(?m)^EnableUPnP = .*','EnableUPnP = False')
Set-Content -LiteralPath (Join-Path $liveSystem 'ppsspp.ini') -Value $liveIni
Copy-Item -LiteralPath $Eboot -Destination (Join-Path $liveRun 'EBOOT.PBP')
Set-Content -LiteralPath (Join-Path $liveRun 'online.cfg') -Value '0 127.0.0.1 35927 1'
Set-Content -LiteralPath (Join-Path $liveRun 'multiplayer-online-live.flag') -Value '1'
$liveServer=$null;$liveProcess=$null
try {
 $liveServer=Start-Process -FilePath $Node -ArgumentList @('"'+(Join-Path $liveRoot 'server/server.mjs')+'"','--host','127.0.0.1','--port','35927') -WorkingDirectory $liveRun -WindowStyle Hidden -RedirectStandardOutput (Join-Path $liveRun 'server.log') -RedirectStandardError (Join-Path $liveRun 'server-error.log') -PassThru
 Start-Sleep -Milliseconds 500
 if($liveServer.HasExited){throw 'Isolated loopback test server did not start'}
 $liveProcess=Start-Process -FilePath (Join-Path $liveRuntime 'PPSSPPWindows64.exe') -ArgumentList ('"'+(Join-Path $liveRun 'EBOOT.PBP')+'"') -WorkingDirectory $liveRun -WindowStyle Hidden -PassThru
 $liveReport=Join-Path $liveRun 'multiplayer-online-live.txt';$liveDeadline=(Get-Date).AddSeconds(120)
 while((Get-Date)-lt $liveDeadline){if((Test-Path -LiteralPath $liveReport)-and(Select-String -LiteralPath $liveReport -Pattern '^RESULT ' -Quiet)){break};Start-Sleep -Milliseconds 250}
 Get-Content -LiteralPath $liveReport
 Write-Output "Review: $liveRun"
 if(!(Select-String -LiteralPath $liveReport -Pattern '^RESULT 0 failures$' -Quiet)){throw 'Live Multiplayer checks failed or timed out'}
}finally{
 if($liveProcess-and !$liveProcess.HasExited){Stop-Process -Id $liveProcess.Id}
 if($liveServer-and !$liveServer.HasExited){Stop-Process -Id $liveServer.Id}
}
