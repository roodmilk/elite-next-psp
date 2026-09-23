param([string]$OutputDir='assets/preview/station-cue-kit')
$ErrorActionPreference='Stop'
Add-Type -AssemblyName System.Drawing

$pal=@{
 VOID=[Drawing.Color]::FromArgb(8,13,24); CHAR=[Drawing.Color]::FromArgb(21,28,39)
 SLATE=[Drawing.Color]::FromArgb(41,54,70); OLIVE=[Drawing.Color]::FromArgb(90,96,76)
 RUST=[Drawing.Color]::FromArgb(139,75,55); OCHRE=[Drawing.Color]::FromArgb(193,139,77)
 CREAM=[Drawing.Color]::FromArgb(229,210,163); CYAN=[Drawing.Color]::FromArgb(85,212,212)
 AMBER=[Drawing.Color]::FromArgb(240,180,91); DANGER=[Drawing.Color]::FromArgb(200,90,75)
}
$rooms=@(
 @{id=0;label='ARRIVAL';role='CYAN';bg='SLATE';motif='window'},
 @{id=1;label='SHOP';role='OCHRE';bg='OLIVE';motif='wrench'},
 @{id=2;label='CANTEEN';role='AMBER';bg='RUST';motif='mug'},
 @{id=3;label='CARGO';role='OCHRE';bg='OCHRE';motif='cargo'},
 @{id=4;label='GUILD';role='CYAN';bg='SLATE';motif='star'},
 @{id=5;label='CLINIC';role='DANGER';bg='CREAM';motif='cross'},
 @{id=6;label='CUSTOMS';role='CYAN';bg='SLATE';motif='gate'}
)
function CuePixel($b,$x,$y,$c){if($x-ge 0 -and $y-ge 0 -and $x-lt $b.Width -and $y-lt $b.Height){$b.SetPixel($x,$y,$c)}}
function CueRect($b,$x,$y,$w,$h,$c){for($yy=$y;$yy-lt $y+$h;$yy++){for($xx=$x;$xx-lt $x+$w;$xx++){CuePixel $b $xx $yy $c}}}
function CueHLine($b,$x,$y,$w,$c){CueRect $b $x $y $w 1 $c}
function CueVLine($b,$x,$y,$h,$c){CueRect $b $x $y 1 $h $c}
function CueColor($name,$contrast){
 if(!$contrast){return $pal[$name]}
 switch($name){
  'VOID'{return $pal.VOID};'CHAR'{return $pal.VOID};'CYAN'{return $pal.CYAN}
  default{return $pal.CREAM}
 }
}
$glyph=@{
 A=@(6,9,9,15,9,9,9); B=@(14,9,9,14,9,9,14); C=@(7,8,8,8,8,8,7); D=@(14,9,9,9,9,9,14); E=@(15,8,8,14,8,8,15)
 G=@(7,8,8,11,9,9,7); H=@(9,9,9,15,9,9,9); I=@(15,2,2,2,2,2,15); L=@(8,8,8,8,8,8,15)
 M=@(17,27,21,21,17,17,17); N=@(9,13,13,11,11,9,9); O=@(6,9,9,9,9,9,6); P=@(14,9,9,14,8,8,8)
 R=@(14,9,9,14,10,9,9); S=@(7,8,8,6,1,1,14); T=@(15,2,2,2,2,2,2); U=@(9,9,9,9,9,9,6)
 V=@(9,9,9,9,9,6,6); W=@(17,17,17,21,21,27,17)
}
function Text5($b,$x,$y,$s,$ink){foreach($ch in $s.ToCharArray()){if($ch -eq ' '){$x+=4;continue};$rows=$glyph[[string]$ch];if($null -eq $rows){$x+=6;continue};for($yy=0;$yy-lt 7;$yy++){for($xx=0;$xx-lt 4;$xx++){if($rows[$yy] -band (1 -shl (3-$xx))){CuePixel $b ($x+$xx) ($y+$yy) $ink}}};$x+=6}}
function Motif($b,$x,$y,$kind,$ink,$dark){
 $hi=$pal.CREAM
 switch($kind){
  'window'{CueRect $b ($x+2) ($y+2) 28 16 $dark;CueHLine $b ($x+5) ($y+6) 22 $ink;CueVLine $b ($x+15) ($y+5) 12 $ink;CueHLine $b ($x+5) ($y+15) 22 $ink;CuePixel $b ($x+9) ($y+10) $hi;CuePixel $b ($x+22) ($y+8) $hi}
  'wrench'{CueHLine $b ($x+7) ($y+6) 16 $ink;CueVLine $b ($x+14) ($y+4) 10 $ink;CueRect $b ($x+11) ($y+12) 8 3 $ink;CuePixel $b ($x+7) ($y+5) $hi;CuePixel $b ($x+24) ($y+5) $hi}
  'mug'{CueRect $b ($x+8) ($y+7) 14 12 $ink;CueRect $b ($x+22) ($y+9) 5 7 $ink;CueHLine $b ($x+10) ($y+20) 14 $ink;CueVLine $b ($x+11) ($y+2) 4 $hi;CueVLine $b ($x+17) ($y+1) 5 $hi}
  'cargo'{CueRect $b ($x+5) ($y+10) 10 8 $ink;CueRect $b ($x+16) ($y+10) 10 8 $ink;CueRect $b ($x+10) ($y+2) 10 8 $ink;CueHLine $b ($x+4) ($y+20) 24 $hi}
  'star'{CuePixel $b ($x+15) ($y+1) $ink;CueHLine $b ($x+10) ($y+12) 12 $ink;CueVLine $b ($x+15) ($y+6) 14 $ink;CuePixel $b ($x+8) ($y+12) $ink;CuePixel $b ($x+22) ($y+12) $ink}
  'cross'{CueRect $b ($x+13) ($y+2) 6 22 $ink;CueRect $b ($x+5) ($y+10) 22 6 $ink}
  'gate'{CueVLine $b ($x+6) ($y+2) 22 $ink;CueVLine $b ($x+23) ($y+2) 22 $ink;CueHLine $b ($x+9) ($y+3) 14 $ink;CuePixel $b ($x+13) ($y+13) $hi;CuePixel $b ($x+17) ($y+13) $hi}
 }
}
function SmallMotif($b,$x,$y,$kind,$ink,$dark){
 switch($kind){
  'window'{CueRect $b ($x+1) ($y+2) 12 6 $dark;CueHLine $b $x ($y+1) 14 $ink;CueVLine $b ($x+6) $y 9 $ink;CuePixel $b ($x+3) ($y+5) $pal.CREAM}
  'wrench'{CueHLine $b ($x+2) ($y+4) 9 $ink;CueVLine $b ($x+6) ($y+2) 6 $ink;CuePixel $b $x ($y+3) $pal.CREAM;CuePixel $b ($x+11) ($y+3) $pal.CREAM}
  'mug'{CueRect $b ($x+3) ($y+2) 7 6 $ink;CueRect $b ($x+10) ($y+3) 3 4 $ink;CueHLine $b ($x+3) ($y+9) 7 $ink;CueVLine $b ($x+5) $y 2 $pal.CREAM}
  'cargo'{CueRect $b ($x+1) ($y+4) 5 5 $ink;CueRect $b ($x+7) ($y+4) 5 5 $ink;CueRect $b ($x+4) ($y+1) 5 4 $ink}
  'star'{CuePixel $b ($x+6) $y $ink;CueHLine $b ($x+3) ($y+5) 7 $ink;CueVLine $b ($x+6) ($y+2) 7 $ink;CuePixel $b ($x+1) ($y+5) $ink;CuePixel $b ($x+12) ($y+5) $ink}
  'cross'{CueRect $b ($x+5) $y 4 11 $ink;CueRect $b ($x+2) ($y+3) 10 4 $ink}
  'gate'{CueVLine $b ($x+2) $y 10 $ink;CueVLine $b ($x+11) $y 10 $ink;CueHLine $b ($x+4) ($y+1) 6 $ink;CuePixel $b ($x+6) ($y+6) $pal.CREAM}
 }
}
function MakeSign($room,$contrast){$b=[Drawing.Bitmap]::new(80,16,[Drawing.Imaging.PixelFormat]::Format24bppRgb);$bg=if($contrast){$pal.CHAR}else{CueColor $room.bg $false};$labelInk=if(!$contrast -and $room.bg -eq 'CREAM'){$pal.CHAR}else{CueColor 'CREAM' $contrast};CueRect $b 0 0 80 16 $bg;CueRect $b 0 0 80 1 (CueColor 'CREAM' $contrast);CueRect $b 0 15 80 1 (CueColor 'SLATE' $contrast);CueVLine $b 0 0 16 (CueColor 'CREAM' $contrast);CueVLine $b 79 0 16 (CueColor 'SLATE' $contrast);SmallMotif $b 3 2 $room.motif (CueColor $room.role $contrast) (CueColor 'CHAR' $contrast);Text5 $b 21 4 $room.label $labelInk;return $b}
function MakeDoor($room,$contrast){$b=[Drawing.Bitmap]::new(52,60,[Drawing.Imaging.PixelFormat]::Format24bppRgb);$frame=CueColor 'SLATE' $contrast;$dark=CueColor 'CHAR' $contrast;$ink=CueColor $room.role $contrast;CueRect $b 0 0 52 60 $frame;CueRect $b 3 3 46 49 $dark;CueRect $b 7 8 38 37 (CueColor 'VOID' $contrast);CueRect $b 5 3 42 3 $ink;CueRect $b 5 52 42 5 (CueColor 'OCHRE' $contrast);for($i=0;$i-lt 6;$i++){if(($i%2)-eq 0){CueRect $b (6+$i*7) 52 4 5 $dark}};Motif $b 8 13 $room.motif $ink $dark;CueRect $b 8 45 36 2 (CueColor 'SLATE' $contrast);return $b}
function MakeLowDoor($room,$contrast){$b=[Drawing.Bitmap]::new(42,26,[Drawing.Imaging.PixelFormat]::Format24bppRgb);$frame=CueColor 'SLATE' $contrast;$dark=CueColor 'CHAR' $contrast;$ink=CueColor $room.role $contrast;CueRect $b 0 0 42 26 $frame;CueRect $b 3 3 36 17 $dark;CueRect $b 6 6 30 10 (CueColor 'VOID' $contrast);CueRect $b 4 3 34 2 $ink;for($i=0;$i-lt 5;$i++){if(($i%2)-eq 0){CueRect $b (5+$i*7) 21 4 3 (CueColor 'OCHRE' $contrast)}};SmallMotif $b 14 6 $room.motif $ink $dark;return $b}
function MakeRoute($contrast){$b=[Drawing.Bitmap]::new(16,8,[Drawing.Imaging.PixelFormat]::Format24bppRgb);CueRect $b 0 0 16 8 (CueColor 'VOID' $contrast);CueHLine $b 1 3 9 (CueColor 'CYAN' $contrast);CueHLine $b 1 4 9 (CueColor 'CYAN' $contrast);CuePixel $b 10 2 (CueColor 'CREAM' $contrast);CuePixel $b 11 3 (CueColor 'CREAM' $contrast);CueRect $b 12 2 2 4 (CueColor 'CREAM' $contrast);CuePixel $b 14 3 (CueColor 'CREAM' $contrast);CuePixel $b 14 4 (CueColor 'CREAM' $contrast);return $b}
function MakeShipMarker($contrast){$b=[Drawing.Bitmap]::new(24,12,[Drawing.Imaging.PixelFormat]::Format24bppRgb);CueRect $b 0 0 24 12 (CueColor 'VOID' $contrast);$ink=CueColor 'CREAM' $contrast;$glow=CueColor 'AMBER' $contrast;CuePixel $b 4 6 $glow;CueHLine $b 6 5 10 $ink;CueHLine $b 7 6 12 $ink;CueHLine $b 8 7 10 $ink;CuePixel $b 9 4 $ink;CuePixel $b 9 8 $ink;CuePixel $b 18 5 $ink;CuePixel $b 18 7 $ink;CueHLine $b 1 10 5 (CueColor 'CYAN' $contrast);return $b}
function MakeArrivalWindow($contrast){$b=[Drawing.Bitmap]::new(128,54,[Drawing.Imaging.PixelFormat]::Format24bppRgb);$frame=CueColor 'SLATE' $contrast;$dark=CueColor 'VOID' $contrast;$cool=CueColor 'CYAN' $contrast;$warm=CueColor 'OCHRE' $contrast;$cream=CueColor 'CREAM' $contrast;CueRect $b 0 0 128 54 $frame;CueRect $b 4 4 120 46 $dark;CueRect $b 7 7 114 37 (CueColor 'CHAR' $contrast);for($i=0;$i-lt 10;$i++){CuePixel $b (12+$i*9) (10+($i*7)%24) $cool};CueHLine $b 8 44 112 $cool;CueRect $b 42 29 50 4 $warm;CueRect $b 50 25 32 4 $cream;CueRect $b 58 21 14 4 $cream;CueRect $b 44 33 8 2 $cool;CueRect $b 84 33 8 2 $cool;CuePixel $b 39 31 $cool;CuePixel $b 94 31 $cool;return $b}
function Blit($dst,$src,$x,$y){for($yy=0;$yy-lt $src.Height;$yy++){for($xx=0;$xx-lt $src.Width;$xx++){CuePixel $dst ($x+$xx) ($y+$yy) $src.GetPixel($xx,$yy)}}}
New-Item -ItemType Directory -Force $OutputDir | Out-Null
$normal=[Drawing.Bitmap]::new(480,272,[Drawing.Imaging.PixelFormat]::Format24bppRgb);$contrast=[Drawing.Bitmap]::new(480,272,[Drawing.Imaging.PixelFormat]::Format24bppRgb);CueRect $normal 0 0 480 272 $pal.VOID;CueRect $contrast 0 0 480 272 $pal.VOID
for($i=0;$i-lt $rooms.Count;$i++){
 $room=$rooms[$i];$id=$room.id.ToString('00');$sn=MakeSign $room $false;$sc=MakeSign $room $true;$dn=MakeDoor $room $false;$dc=MakeDoor $room $true;$ln=MakeLowDoor $room $false;$lc=MakeLowDoor $room $true
 $sn.Save((Join-Path $OutputDir "room-$id-sign-normal.png"),[Drawing.Imaging.ImageFormat]::Png);$sc.Save((Join-Path $OutputDir "room-$id-sign-contrast.png"),[Drawing.Imaging.ImageFormat]::Png);$dn.Save((Join-Path $OutputDir "room-$id-door-normal.png"),[Drawing.Imaging.ImageFormat]::Png);$dc.Save((Join-Path $OutputDir "room-$id-door-contrast.png"),[Drawing.Imaging.ImageFormat]::Png);$ln.Save((Join-Path $OutputDir "room-$id-low-door-normal.png"),[Drawing.Imaging.ImageFormat]::Png);$lc.Save((Join-Path $OutputDir "room-$id-low-door-contrast.png"),[Drawing.Imaging.ImageFormat]::Png)
 $col=($i%4)*120;$row=[Math]::Floor($i/4)*126;Blit $normal $sn ($col+18) ($row+6);Blit $normal $dn ($col+32) ($row+30);Blit $contrast $sc ($col+18) ($row+6);Blit $contrast $dc ($col+32) ($row+30)
 $sn.Dispose();$sc.Dispose();$dn.Dispose();$dc.Dispose();$ln.Dispose();$lc.Dispose()
}
$normal.Save((Join-Path $OutputDir 'station-cue-kit-normal-native.png'),[Drawing.Imaging.ImageFormat]::Png);$contrast.Save((Join-Path $OutputDir 'station-cue-kit-contrast-native.png'),[Drawing.Imaging.ImageFormat]::Png);$normal.Dispose();$contrast.Dispose()
$routeN=MakeRoute $false;$routeC=MakeRoute $true;$shipN=MakeShipMarker $false;$shipC=MakeShipMarker $true;$winN=MakeArrivalWindow $false;$winC=MakeArrivalWindow $true
$routeN.Save((Join-Path $OutputDir 'route-marker-normal.png'),[Drawing.Imaging.ImageFormat]::Png);$routeC.Save((Join-Path $OutputDir 'route-marker-contrast.png'),[Drawing.Imaging.ImageFormat]::Png);$shipN.Save((Join-Path $OutputDir 'ship-return-marker-normal.png'),[Drawing.Imaging.ImageFormat]::Png);$shipC.Save((Join-Path $OutputDir 'ship-return-marker-contrast.png'),[Drawing.Imaging.ImageFormat]::Png);$winN.Save((Join-Path $OutputDir 'arrival-window-normal.png'),[Drawing.Imaging.ImageFormat]::Png);$winC.Save((Join-Path $OutputDir 'arrival-window-contrast.png'),[Drawing.Imaging.ImageFormat]::Png)
$wn=[Drawing.Bitmap]::new(480,272,[Drawing.Imaging.PixelFormat]::Format24bppRgb);$wc=[Drawing.Bitmap]::new(480,272,[Drawing.Imaging.PixelFormat]::Format24bppRgb);CueRect $wn 0 0 480 272 $pal.VOID;CueRect $wc 0 0 480 272 $pal.VOID;Blit $wn $winN 12 10;Blit $wc $winC 12 10;for($i=0;$i-lt 6;$i++){Blit $wn $routeN (12+$i*24) 76;Blit $wc $routeC (12+$i*24) 76};Blit $wn $shipN 12 100;Blit $wc $shipC 12 100;Text5 $wn 44 101 'SHIP RETURN' $pal.CREAM;Text5 $wc 44 101 'SHIP RETURN' $pal.CREAM;Text5 $wn 12 124 'ARRIVAL WINDOW / BERTH SILHOUETTE' $pal.CREAM;Text5 $wc 12 124 'ARRIVAL WINDOW / BERTH SILHOUETTE' $pal.CREAM;$wn.Save((Join-Path $OutputDir 'wayfinding-normal-native.png'),[Drawing.Imaging.ImageFormat]::Png);$wc.Save((Join-Path $OutputDir 'wayfinding-contrast-native.png'),[Drawing.Imaging.ImageFormat]::Png);$wn.Dispose();$wc.Dispose();$routeN.Dispose();$routeC.Dispose();$shipN.Dispose();$shipC.Dispose();$winN.Dispose();$winC.Dispose()
$lowerN=[Drawing.Bitmap]::new(480,272,[Drawing.Imaging.PixelFormat]::Format24bppRgb);$lowerC=[Drawing.Bitmap]::new(480,272,[Drawing.Imaging.PixelFormat]::Format24bppRgb);CueRect $lowerN 0 0 480 272 $pal.VOID;CueRect $lowerC 0 0 480 272 $pal.VOID;for($i=0;$i-lt $rooms.Count;$i++){$room=$rooms[$i];$ln=MakeLowDoor $room $false;$lc=MakeLowDoor $room $true;$col=($i%4)*120;$row=[Math]::Floor($i/4)*80;Blit $lowerN $ln ($col+38) ($row+8);Blit $lowerC $lc ($col+38) ($row+8);$ln.Dispose();$lc.Dispose()};$lowerN.Save((Join-Path $OutputDir 'lower-door-normal-native.png'),[Drawing.Imaging.ImageFormat]::Png);$lowerC.Save((Join-Path $OutputDir 'lower-door-contrast-native.png'),[Drawing.Imaging.ImageFormat]::Png);$lowerN.Dispose();$lowerC.Dispose()
Write-Output "Baked 7 room signs, full/lower doorway cues and wayfinding markers with normal/high-contrast variants; native proof sheets are 480x272"
