param([Parameter(Mandatory=$true)][string]$Eboot,[string]$Emulator="$PSScriptRoot/../../../work/ppsspp/PPSSPPWindows64.exe")
$ErrorActionPreference='Stop'
$freightRoot=Split-Path $PSScriptRoot -Parent
$freightDir=Join-Path $freightRoot ('outputs/deep-freighter-review-'+(Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
New-Item -ItemType Directory -Path $freightDir | Out-Null
$freightExe=(Resolve-Path -LiteralPath $Emulator).Path
Add-Type -AssemblyName System.Drawing
$cases=@(@(7,0,0),@(31,1,0),@(127,2,0),@(173,0,0),@(7,0,1))
foreach($case in $cases){
 $system=$case[0];$lane=$case[1];$blocked=$case[2];$name="system-$system-lane-$lane-blocked-$blocked";$run=Join-Path $freightDir $name
 New-Item -ItemType Directory -Path $run | Out-Null
 Copy-Item -LiteralPath $Eboot -Destination (Join-Path $run 'EBOOT.PBP')
 [IO.File]::WriteAllText((Join-Path $run 'open-deep-freighter.flag'),"$system $lane $blocked")
 [IO.File]::WriteAllText((Join-Path $run 'dump-native.flag'),'1')
 $process=Start-Process -FilePath $freightExe -ArgumentList ('"'+(Join-Path $run 'EBOOT.PBP')+'"') -WorkingDirectory $run -WindowStyle Hidden -PassThru
 try{
  $deadline=(Get-Date).AddSeconds(35);$bmp=Join-Path $run 'native-480x272.bmp'
  while((Get-Date) -lt $deadline){if((Test-Path -LiteralPath $bmp) -and (Get-Item -LiteralPath $bmp).Length -ge 391734){break};Start-Sleep -Milliseconds 200}
  if(!(Test-Path -LiteralPath $bmp) -or (Get-Item -LiteralPath $bmp).Length -lt 391734){throw "No complete capture for $name"}
  $image=[Drawing.Image]::FromFile($bmp);try{$image.Save((Join-Path $freightDir ($name+'.png')),[Drawing.Imaging.ImageFormat]::Png)}finally{$image.Dispose()}
 }finally{if(!$process.HasExited){Stop-Process -Id $process.Id}}
}
Write-Output "Deep freighter review: $freightDir"
