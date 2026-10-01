param([string]$ProbeRoot,[string]$MusicFolder)
$ErrorActionPreference='Stop'
$probeEmulator=(Resolve-Path "$PSScriptRoot/../../../work/ppsspp/PPSSPPWindows64.exe").Path
foreach($track in @('spacebattle','spacebattle2','spacebattle3','spacebattle4')){
 foreach($mode in @('reference','game')){
  $dir=Join-Path $ProbeRoot "$track-$mode"
  New-Item -ItemType Directory -Path $dir | Out-Null
  Copy-Item -LiteralPath "$ProbeRoot/$mode/EBOOT.PBP" -Destination $dir
  Copy-Item -LiteralPath "$MusicFolder/$track.ogg" -Destination "$dir/input.ogg"
  $process=Start-Process -FilePath $probeEmulator -ArgumentList ('"'+$dir+'\EBOOT.PBP"') -WorkingDirectory $dir -WindowStyle Hidden -PassThru
  try{
   $deadline=(Get-Date).AddSeconds(25)
   do{Start-Sleep -Milliseconds 200;$report=Get-Item -LiteralPath "$dir/probe.txt" -ErrorAction SilentlyContinue}while((!$report -or !$report.Length) -and (Get-Date) -lt $deadline)
   if(!$report -or !$report.Length){throw "Silent decoder timeout: $track $mode"}
   Write-Output "$track $mode $(Get-Content -LiteralPath $report.FullName)"
  }finally{if(!$process.HasExited){Stop-Process -Id $process.Id}}
 }
}
