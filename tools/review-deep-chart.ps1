param([string]$Emulator="$PSScriptRoot/../../../work/ppsspp/PPSSPPWindows64.exe",[string]$Eboot="$PSScriptRoot/../EBOOT.PBP")
$ErrorActionPreference='Stop'
$chartRoot=Split-Path $PSScriptRoot -Parent
$chartDir=Join-Path $chartRoot ('outputs/chart-review-'+(Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
New-Item -ItemType Directory -Path $chartDir | Out-Null
Copy-Item -LiteralPath $Eboot -Destination (Join-Path $chartDir 'EBOOT.PBP')
[IO.File]::WriteAllText((Join-Path $chartDir 'chart-review.flag'),'1')
$chartExe=(Resolve-Path -LiteralPath $Emulator).Path
$chartProcess=Start-Process -FilePath $chartExe -ArgumentList ('"'+(Join-Path $chartDir 'EBOOT.PBP')+'"') -WorkingDirectory $chartDir -WindowStyle Hidden -PassThru
try {
 $chartDeadline=(Get-Date).AddSeconds(55)
 $chartReport=Join-Path $chartDir 'chart-review.txt'
 while((Get-Date) -lt $chartDeadline){
  if((Test-Path -LiteralPath $chartReport) -and (Select-String -LiteralPath $chartReport -Pattern '^RESULT ' -Quiet)){break}
  Start-Sleep -Milliseconds 250
 }
 if(!(Test-Path -LiteralPath $chartReport)){throw 'Chart capture produced no report.'}
 Get-Content -LiteralPath $chartReport
 Add-Type -AssemblyName System.Drawing
 foreach($chartName in (@('chart-all','chart-mega','chart-zoom','chart-search')+@(0..8 | ForEach-Object {"chart-filter-$_"}))){
  $chartImage=[Drawing.Image]::FromFile((Join-Path $chartDir ($chartName+'.bmp')))
  try{$chartImage.Save((Join-Path $chartDir ($chartName+'.png')),[Drawing.Imaging.ImageFormat]::Png)}finally{$chartImage.Dispose()}
 }
 Write-Output "Captures: $chartDir"
 if(!(Select-String -LiteralPath $chartReport -Pattern '^RESULT 0 failures$' -Quiet)){throw 'Chart checks failed.'}
} finally {
 if(!$chartProcess.HasExited){Stop-Process -Id $chartProcess.Id}
}
