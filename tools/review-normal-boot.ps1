param([string]$Eboot="$PSScriptRoot/../EBOOT.PBP",[string]$Emulator="$PSScriptRoot/../../../work/ppsspp/PPSSPPWindows64.exe")
$ErrorActionPreference='Stop'
$bootRun=Join-Path (Split-Path $PSScriptRoot -Parent) ('outputs/normal-boot-'+(Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
New-Item -ItemType Directory -Path $bootRun | Out-Null
Copy-Item -LiteralPath $Eboot -Destination (Join-Path $bootRun 'EBOOT.PBP')
Set-Content -LiteralPath (Join-Path $bootRun 'dump-native.flag') -Value 1
$bootProcess=Start-Process -FilePath (Resolve-Path -LiteralPath $Emulator).Path -ArgumentList @('--log="'+(Join-Path $bootRun 'emulator.log')+'"','"'+(Join-Path $bootRun 'EBOOT.PBP')+'"') -WorkingDirectory $bootRun -WindowStyle Hidden -PassThru
try {
 $bootDeadline=(Get-Date).AddSeconds(35);$bootReady=$false
 while((Get-Date) -lt $bootDeadline){
  if($bootProcess.HasExited){throw 'Normal startup exited before producing a frame'}
  $bootFrame=Get-Item -LiteralPath (Join-Path $bootRun 'native-480x272.bmp') -ErrorAction SilentlyContinue
  # Creation happens before the frame is written: do not stop on a zero-byte file.
  if($bootFrame -and $bootFrame.Length -eq 391734){$bootReady=$true;break}
  Start-Sleep -Milliseconds 250
 }
 if(!$bootReady){throw "Normal startup timed out; see $bootRun"}
 Start-Sleep -Milliseconds 750
 $bootBytes=[IO.File]::ReadAllBytes($bootFrame.FullName)
 if([BitConverter]::ToInt32($bootBytes,18) -ne 480 -or [BitConverter]::ToInt32($bootBytes,22) -ne 272){throw 'Invalid native dimensions'}
 $bootDistinct=@{};for($i=54;$i -lt $bootBytes.Length -and $bootDistinct.Count -lt 32;$i++){$bootDistinct[$bootBytes[$i]]=1}
 if($bootDistinct.Count -lt 32){throw 'Startup frame is blank'}
 if(Select-String -LiteralPath (Join-Path $bootRun 'emulator.log') -Pattern 'MEMBLOCK_ALLOC_FAILED' -Quiet){throw 'Startup heap allocation failed'}
 $bootResult=@('PASS normal startup reaches a complete nonblank native 480x272 frame','PASS no startup heap allocation failure','RESULT 0 failures')
 $bootResult | Set-Content -LiteralPath (Join-Path $bootRun 'normal-boot-check.txt')
 $bootResult;Write-Output "Review: $bootRun"
}finally{if(!$bootProcess.HasExited){Stop-Process -Id $bootProcess.Id}}
