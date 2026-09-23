param([Parameter(Mandatory=$true)][string]$Source)
$ErrorActionPreference='Stop'
Add-Type -AssemblyName System.Drawing
$image=[Drawing.Bitmap]::new((Resolve-Path $Source).Path)
if($image.Width -ne 480 -or $image.Height -ne 272){throw 'Expected native480x272 concept export'}
$colours=@('080d18','151c27','293646','5a604c','8b4b37','c18b4d','e5d2a3','9b9aa5','55d4d4','f0b45b','c85a4b')
$lookup=@{}
for($i=0;$i -lt $colours.Count;$i++){$lookup[[Drawing.ColorTranslator]::FromHtml('#'+$colours[$i]).ToArgb()]=$i}
$bytes=[Collections.Generic.List[byte]]::new()
for($y=20;$y -lt 188;$y++){for($x=6;$x -lt 346;$x+=2){
 $a=$image.GetPixel($x,$y).ToArgb();$b=$image.GetPixel($x+1,$y).ToArgb()
 if(!$lookup.ContainsKey($a)-or!$lookup.ContainsKey($b)){throw 'Unexpected non-palette pixel'}
 $bytes.Add([byte](($lookup[$a] -shl 4)-bor$lookup[$b]))
}}
$image.Dispose()
$path=Join-Path $PSScriptRoot '../src/generated/second-shift-pixels.h'
New-Item -ItemType Directory -Force (Split-Path $path) | Out-Null
$writer=[IO.StreamWriter]::new($path,$false,[Text.UTF8Encoding]::new($false))
try{
 $writer.WriteLine('/* Generated native340x168,4bit palette indices. Source: assets/second-shift-native.png. */')
 $writer.WriteLine('static const unsigned char second_shift_pixels[28560]={')
 for($i=0;$i -lt $bytes.Count;$i+=24){
  $last=[Math]::Min($i+23,$bytes.Count-1)
  $writer.WriteLine(' '+(($bytes[$i..$last] | ForEach-Object {'0x{0:x2}' -f $_}) -join ',')+',')
 }
 $writer.WriteLine('};')
}finally{$writer.Dispose()}
Write-Output "Baked $($bytes.Count) bytes;340x168;11colour;4bit;native1x"
