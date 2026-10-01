param([Parameter(Mandatory=$true)][string]$Eboot,[int[]]$Systems=@(7,31,63,127,173,255),[string]$Emulator="$PSScriptRoot/../../../work/ppsspp/PPSSPPWindows64.exe")
$ErrorActionPreference='Stop'
$skyRoot=Split-Path $PSScriptRoot -Parent
$skyDir=Join-Path $skyRoot ('outputs/soft-sky-review-'+(Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
New-Item -ItemType Directory -Path $skyDir | Out-Null
$skyExe=(Resolve-Path -LiteralPath $Emulator).Path
Add-Type -AssemblyName System.Drawing
foreach($skySystem in $Systems){
 $skyCase=Join-Path $skyDir "system-$skySystem"
 New-Item -ItemType Directory -Path $skyCase | Out-Null
 Copy-Item -LiteralPath $Eboot -Destination (Join-Path $skyCase 'EBOOT.PBP')
 [IO.File]::WriteAllText((Join-Path $skyCase 'soft-sky-review.flag'),[string]$skySystem)
 $skyProcess=Start-Process -FilePath $skyExe -ArgumentList ('"'+(Join-Path $skyCase 'EBOOT.PBP')+'"') -WorkingDirectory $skyCase -WindowStyle Hidden -PassThru
 try {
  $skyDeadline=(Get-Date).AddSeconds($(if($skySystem -eq 256){100}else{55}))
  $skyReport=Join-Path $skyCase 'soft-sky-review.txt'
  while((Get-Date) -lt $skyDeadline){
   if((Test-Path -LiteralPath $skyReport) -and (Select-String -LiteralPath $skyReport -Pattern '^RESULT ' -Quiet)){break}
   Start-Sleep -Milliseconds 250
  }
  if(!(Test-Path -LiteralPath $skyReport)){throw "No report for system $skySystem"}
  Get-Content -LiteralPath $skyReport
  foreach($skyCapture in (Get-ChildItem -LiteralPath $skyCase -Filter '*.bmp')){
   $skyImage=[Drawing.Image]::FromFile($skyCapture.FullName)
   try{$skyImage.Save([IO.Path]::ChangeExtension($skyCapture.FullName,'.png'),[Drawing.Imaging.ImageFormat]::Png)}finally{$skyImage.Dispose()}
  }
  if(!(Select-String -LiteralPath $skyReport -Pattern '^RESULT 0 failures$' -Quiet)){throw "Review failed for system $skySystem"}
 } finally {if(!$skyProcess.HasExited){Stop-Process -Id $skyProcess.Id}}
}
Write-Output "Review: $skyDir"
