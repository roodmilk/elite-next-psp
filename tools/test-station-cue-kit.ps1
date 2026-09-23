$ErrorActionPreference='Stop'
Add-Type -AssemblyName System.Drawing
$root=Join-Path (Get-Location) 'assets/preview/station-cue-kit'
$manifest=Get-Content (Join-Path $root 'STATION-CUE-KIT.json') -Raw | ConvertFrom-Json
$expected=@{
 'sign'=@(80,16);'door'=@(52,60);'low-door'=@(42,26);'route-marker'=@(16,8);'ship-return-marker'=@(24,12);'arrival-window'=@(128,54)
}
$allowed=@('8,13,24','21,28,39','41,54,70','90,96,76','139,75,55','193,139,77','229,210,163','155,154,165','85,212,212','240,180,91','200,90,75')
$files=0
foreach($room in $manifest.rooms){
 foreach($kind in 'sign','door','low-door'){
  $size=$expected[$kind]
  foreach($variant in 'normal','contrast'){
   $prop=if($kind -eq 'low-door'){"${variant}_low_door"}else{"${variant}_${kind}"}; $file=$room.$prop; $path=Join-Path $root $file
   if(!(Test-Path $path)){throw "Missing $file"}
   $b=[Drawing.Bitmap]::new($path)
   if($b.Width-ne$size[0]-or$b.Height-ne$size[1]){throw "$file is $($b.Width)x$($b.Height), expected $($size[0])x$($size[1])"}
   $colors=[Collections.Generic.HashSet[string]]::new()
   for($y=0;$y-lt$b.Height;$y++){for($x=0;$x-lt$b.Width;$x++){[void]$colors.Add("$($b.GetPixel($x,$y).R),$($b.GetPixel($x,$y).G),$($b.GetPixel($x,$y).B)")}}
   foreach($color in $colors){if($allowed-notcontains$color){throw "$file uses non-canonical color $color"}}
   $b.Dispose();$files++
  }
 }
}
foreach($pair in @(
 @{base='route-marker';size=@(16,8)},@{base='ship-return-marker';size=@(24,12)},@{base='arrival-window';size=@(128,54)}
)){
 foreach($variant in 'normal','contrast'){
  $file="$($pair.base)-$variant.png";$path=Join-Path $root $file;if(!(Test-Path $path)){throw "Missing $file"};$b=[Drawing.Bitmap]::new($path);if($b.Width-ne$pair.size[0]-or$b.Height-ne$pair.size[1]){throw "$file has wrong size"};$b.Dispose();$files++
 }
}
foreach($proof in 'station-cue-kit-normal-native.png','station-cue-kit-contrast-native.png','wayfinding-normal-native.png','wayfinding-contrast-native.png','lower-door-normal-native.png','lower-door-contrast-native.png'){$b=[Drawing.Bitmap]::new((Join-Path $root $proof));if($b.Width-ne480-or$b.Height-ne272){throw "$proof is not 480x272"};$b.Dispose()}
if($manifest.rooms.Count-ne7){throw "Expected seven room records"}
Write-Output "PASS station cue kit: $files native assets, 7 room bindings, 480x272 proof sheets, canonical palette"
