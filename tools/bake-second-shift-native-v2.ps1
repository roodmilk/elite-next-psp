param(
 [string]$Source='assets/second-shift-native-v2-source.png',
 [string]$BaseReview='assets/second-shift-native.png',
 [string]$Review='assets/second-shift-native-v2.png',
 [string]$Header='src/generated/second-shift-pixels.h'
)
$ErrorActionPreference='Stop'
Add-Type -AssemblyName System.Drawing
Add-Type -AssemblyName System.Drawing.Common -ErrorAction SilentlyContinue
$palette=@(
 [Drawing.Color]::FromArgb(8,13,24),[Drawing.Color]::FromArgb(21,28,39),
 [Drawing.Color]::FromArgb(41,54,70),[Drawing.Color]::FromArgb(90,96,76),
 [Drawing.Color]::FromArgb(139,75,55),[Drawing.Color]::FromArgb(193,139,77),
 [Drawing.Color]::FromArgb(229,210,163),[Drawing.Color]::FromArgb(155,154,165),
 [Drawing.Color]::FromArgb(85,212,212),[Drawing.Color]::FromArgb(240,180,91),
 [Drawing.Color]::FromArgb(200,90,75)
)
function Closest([Drawing.Color]$c){
 $best=0;$distance=[double]::MaxValue
 for($i=0;$i-lt$palette.Count;$i++){
  $dr=$c.R-$palette[$i].R;$dg=$c.G-$palette[$i].G;$db=$c.B-$palette[$i].B
  $d=$dr*$dr+$dg*$dg+$db*$db
  if($d-lt$distance){$distance=$d;$best=$i}
 }
 return $best
}
function MakeNative([Drawing.Bitmap]$src){
 $dst=[Drawing.Bitmap]::new(480,272,[Drawing.Imaging.PixelFormat]::Format24bppRgb)
 $g=[Drawing.Graphics]::FromImage($dst)
 $g.InterpolationMode=[Drawing.Drawing2D.InterpolationMode]::NearestNeighbor
 $g.PixelOffsetMode=[Drawing.Drawing2D.PixelOffsetMode]::Half
 $g.CompositingMode=[Drawing.Drawing2D.CompositingMode]::SourceCopy
 $g.DrawImage($src,[Drawing.Rectangle]::new(0,0,480,272))
 $g.Dispose()
 for($y=0;$y-lt272;$y++){for($x=0;$x-lt480;$x++){
  $dst.SetPixel($x,$y,$palette[(Closest $dst.GetPixel($x,$y))])
 }}
 return $dst
}
$sourceBitmap=[Drawing.Bitmap]::new((Resolve-Path $Source).Path)
if(($sourceBitmap.Width -ne 1665) -or ($sourceBitmap.Height -ne 944)){throw "Expected the reviewed 1665x944 1950s pixel-art source; got $($sourceBitmap.Width)x$($sourceBitmap.Height)"}
$nativeBitmap=MakeNative $sourceBitmap
$sourceBitmap.Dispose()
$baseBitmap=[Drawing.Bitmap]::new((Resolve-Path $BaseReview).Path)
for($y=20;$y-lt188;$y++){for($x=6;$x-lt346;$x++){$baseBitmap.SetPixel($x,$y,$nativeBitmap.GetPixel($x,$y))}}
New-Item -ItemType Directory -Force (Split-Path $Review) | Out-Null
$baseBitmap.Save((Join-Path (Get-Location) $Review),[Drawing.Imaging.ImageFormat]::Png)
$baseBitmap.Dispose()
$bytes=[Collections.Generic.List[byte]]::new()
for($y=20;$y-lt188;$y++){for($x=6;$x-lt346;$x+=2){
 $a=$nativeBitmap.GetPixel($x,$y);$b=$nativeBitmap.GetPixel($x+1,$y)
 $left=Closest $a;$right=Closest $b
 $bytes.Add([byte](($left -shl 4) -bor $right))
}}
$nativeBitmap.Dispose()
$writer=[IO.StreamWriter]::new((Join-Path (Get-Location) $Header),$false,[Text.UTF8Encoding]::new($false))
try{
 $writer.WriteLine('/* Generated native340x168,4bit palette indices. Source: assets/second-shift-native-v2-source.png. */')
 $writer.WriteLine('static const unsigned char second_shift_pixels[28560]={')
 for($i=0;$i-lt$bytes.Count;$i+=24){
  $last=[Math]::Min($i+23,$bytes.Count-1)
  $writer.WriteLine(' '+(($bytes[$i..$last] | ForEach-Object {'0x{0:x2}' -f $_}) -join ',')+',')
 }
 $writer.WriteLine('};')
}finally{$writer.Dispose()}
Write-Output "Baked native 480x272 review and 340x168 runtime art; $($bytes.Count) packed bytes"
