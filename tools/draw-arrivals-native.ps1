$ErrorActionPreference='Stop'; Add-Type -AssemblyName System.Drawing
$root=(Resolve-Path (Join-Path $PSScriptRoot '..')).Path
$palette=@('080d18','151c27','293646','5a604c','8b4b37','c18b4d','e5d2a3','9b9aa5','55d4d4','f0b45b','c85a4b') | ForEach-Object {[Drawing.ColorTranslator]::FromHtml('#'+$_)}
$glyphs=[regex]::Matches((Get-Content (Join-Path $root 'src/font8.h') -Raw),'0x([0-9a-fA-F]{2})u') | ForEach-Object {[Convert]::ToInt32($_.Groups[1].Value,16)}
$bmp=[Drawing.Bitmap]::new(480,272);$g=[Drawing.Graphics]::FromImage($bmp);$g.SmoothingMode=[Drawing.Drawing2D.SmoothingMode]::None;$g.InterpolationMode=[Drawing.Drawing2D.InterpolationMode]::NearestNeighbor;$g.Clear($palette[0])
function Box($x,$y,$w,$h,$c){$b=[Drawing.SolidBrush]::new($palette[$c]);$g.FillRectangle($b,$x,$y,$w,$h);$b.Dispose()}
function Line($x1,$y1,$x2,$y2,$c,$width=1){$p=[Drawing.Pen]::new($palette[$c],$width);$g.DrawLine($p,$x1,$y1,$x2,$y2);$p.Dispose()}
function Poly($points,$c){$b=[Drawing.SolidBrush]::new($palette[$c]);$g.FillPolygon($b,$points);$b.Dispose()}
function Pix($x,$y,$c){if($x-ge0-and$x-lt480-and$y-ge0-and$y-lt272){$bmp.SetPixel($x,$y,$palette[$c])}}
function Label($x,$y,[string]$s,$c){for($n=0;$n-lt$s.Length;$n++){ $ch=[int][char]$s[$n];if($ch-lt32-or$ch-gt126){$ch=63};for($r=0;$r-lt8;$r++){for($q=0;$q-lt8;$q++){if(($glyphs[($ch-32)*8+$r]-shr(7-$q))-band1){Pix ($x+$n*8+$q) ($y+$r) $c}}}}}
# UI frame and room slab.
Box 0 0 480 272 0;Box 0 0 480 20 1;Box 0 19 480 1 5;Box 6 20 340 168 0
# Native authored room: deep concourse, ribbed arch, berth window and layered floor.
Box 14 28 326 132 2;Box 20 34 312 98 1;Box 33 42 285 76 0
for($x=24;$x-lt340;$x+=28){Poly @([Drawing.Point]::new($x,26),[Drawing.Point]::new($x+8,26),[Drawing.Point]::new($x+34,116),[Drawing.Point]::new($x+25,116)) 2;Line ($x+7) 29 ($x+30) 112 5}
Box 28 43 48 73 1;Box 34 50 36 56 2;for($r=0;$r-lt7;$r++){Box 38 (55+$r*7) 26 3 $(if($r%2){5}else{6});Pix (42+$r%4*6) (56+$r*7) 8}
# Window and distant planet.
Box 92 38 239 83 1;Box 99 45 225 69 2;Box 106 52 211 55 0
for($s=0;$s-lt34;$s++){Pix (112+(($s*37)%194)) (57+(($s*17)%43)) $(if($s%5 -eq 0){6}else{7})}
for($i=0;$i-lt18;$i++){Pix (247+(($i*11)%35)) (59+(($i*7)%41)) $(if($i%4 -eq 0){8}else{6})}
# Planet at native size.
for($dy=-22;$dy-le22;$dy++){for($dx=-22;$dx-le22;$dx++){if($dx*$dx+$dy*$dy-le484){$c=if($dy-lt-8){6}elseif($dx+2*$dy-gt12){2}else{8};Pix (283+$dx) (75+$dy) $c}}}
for($i=0;$i-lt11;$i++){Line (267+$i*3) (64+($i%4)*6) (274+$i*3) (62+($i%4)*6) 7}
# Arriving freighter: large ochre silhouette over the berth lights.
Poly @([Drawing.Point]::new(125,87),[Drawing.Point]::new(151,76),[Drawing.Point]::new(268,76),[Drawing.Point]::new(309,90),[Drawing.Point]::new(267,105),[Drawing.Point]::new(151,105)) 5
Poly @([Drawing.Point]::new(151,78),[Drawing.Point]::new(221,69),[Drawing.Point]::new(266,78),[Drawing.Point]::new(238,87),[Drawing.Point]::new(154,87)) 6
Box 166 88 102 12 4;Box 177 91 84 6 2;for($i=0;$i-lt9;$i++){Box (180+$i*9) 93 4 2 $(if($i%3 -eq 0){8}else{9})};Poly @([Drawing.Point]::new(126,87),[Drawing.Point]::new(109,93),[Drawing.Point]::new(126,99)) 4;Box 234 77 3 8 8
# Native authored detail pass: human-readable silhouettes, lamps, berth machinery and floor rhythm.
Box 82 41 7 78 5;Box 86 44 2 72 6;Box 329 42 7 76 5;Box 326 47 2 65 6
for($i=0;$i-lt7;$i++){Box (108+$i*28) (108+($i%2)*3) 13 3 6;Pix (112+$i*28) (109+($i%2)*3) 8}
Box 117 47 4 20 9;Box 114 43 10 4 9;Box 199 48 4 20 9;Box 196 44 10 4 9;Box 302 46 4 20 9;Box 299 42 10 4 9
# Freighter panel clusters and engine glow.
Box 145 82 112 4 6;Box 159 80 66 3 7;Box 228 84 33 3 4;Box 152 96 102 3 4;Box 158 101 90 2 6
for($i=0;$i-lt12;$i++){Box (164+($i%6)*15) (87+([int]($i/6))*7) 6 3 $(if($i%3 -eq 0){8}else{9})}
Box 129 89 10 12 4;Box 126 92 4 6 5;Box 256 88 15 13 4;Box 270 92 9 5 8
# Clerk: hair, face, uniform and hand-held arrival slate at native pixel clusters.
Box 43 78 20 8 4;Box 39 83 27 10 4;Box 45 88 15 15 6;Box 48 91 3 3 0;Box 57 91 3 3 0;Box 45 101 16 6 7
Box 37 106 31 32 3;Box 41 109 23 25 5;Box 45 109 15 4 6;Box 34 114 8 19 3;Box 62 114 8 19 3;Box 37 128 10 4 6;Box 60 128 10 4 6;Box 48 118 10 3 4
Box 72 95 21 30 2;Box 75 99 15 22 1;Box 77 101 11 2 8;Box 78 107 8 2 6;Box 78 114 8 2 6
# Traveler: hat, pack, face, coat, boots and case.
Box 286 84 20 5 4;Box 290 80 13 6 4;Box 293 88 14 12 7;Pix 296 93 0;Pix 302 93 0;Box 290 99 22 8 6
Box 283 106 31 38 2;Box 288 107 20 31 3;Box 292 111 13 18 1;Box 278 112 9 22 5;Box 309 112 9 22 5;Box 284 133 10 17 4;Box 302 133 10 17 4;Box 318 130 15 21 5;Box 321 133 9 8 6;Box 321 144 10 3 6
# Center dockhand scale marker, foreground machinery and depth glints.
Box 183 100 15 6 4;Box 186 106 9 11 7;Box 181 117 19 25 1;Box 184 120 13 3 5;Box 182 140 7 12 4;Box 193 140 7 12 4
Box 210 123 22 21 1;Box 214 127 14 12 2;Box 217 130 8 4 8;Box 236 124 24 20 2;Box 240 129 16 10 1;Box 243 132 9 3 9
# Dock rail and activity beyond it.
Box 100 115 230 5 5;Box 102 120 226 3 3;for($x=112;$x-lt330;$x+=27){Box $x 113 3 40 5;Box ($x+1) 120 1 33 6};for($i=0;$i-lt9;$i++){Box (119+$i*23) (126+($i%3)*4) 9 4 $(if($i%2){5}else{6})}
# Clerk at left, native pixel silhouette and face.
Box 48 84 7 11 6;Box 45 92 14 11 9;Box 39 102 28 26 3;Box 42 106 22 18 5;Box 45 111 16 3 6;Box 48 84 5 6 6;Pix 49 88 4;Pix 53 89 4;Box 37 126 34 5 1;Box 40 132 8 18 4;Box 60 132 8 18 4;Box 47 127 12 4 7
# Traveler with case on right.
Box 291 98 7 9 7;Box 288 106 14 12 3;Box 283 115 24 30 2;Box 286 118 18 18 1;Box 278 124 8 4 5;Box 304 124 8 4 5;Box 279 143 12 12 4;Box 299 143 12 12 4;Box 316 137 14 18 5;Box 319 140 8 8 6
# Smaller dockhand silhouette gives scale without crowd simulation.
Box 186 105 4 7 7;Box 183 111 10 14 3;Box 180 123 17 20 1;Box 182 126 13 3 5;Box 181 142 6 11 4;Box 191 142 6 11 4
# Foreground counter, lamp and worn pixel clusters.
Box 8 145 164 12 5;Box 8 157 164 28 1;Box 12 150 150 4 6;for($i=0;$i-lt28;$i++){Pix (12+(($i*19)%148)) (160+(($i*11)%21)) $(if($i%5 -eq 0){4}else{2})};Box 72 127 3 18 9;Box 67 124 14 4 9;for($i=0;$i-lt8;$i++){Pix (68+$i*2) 126 6}
# Keep dynamic SHIP slot clear and show side hatches.
Box 10 62 28 51 1;Box 14 66 20 43 0;Box 315 59 25 52 1;Box 319 63 17 44 0;Box 274 158 68 26 0;Box 274 158 68 1 8;Box 274 183 68 1 8;Box 274 158 1 26 8;Box 341 158 1 26 8;Label 280 168 'SHIP >' 8
# Native options + feedback using the real 8x8 font.
Box 354 20 120 168 1;Box 354 20 1 168 5;Box 354 20 120 1 5;Label 360 24 'OPTIONS' 7;Box 356 31 116 11 5;$items=@('VENN','DOCKHAND','-> CANTEEN','-> CARGO BAY','YOUR SHIP');for($i=0;$i-lt$items.Count;$i++){Label 360 ([int][Math]::Floor((36+$i*12)/8)*8) $items[$i] $(if($i-eq0){9}else{6})}
Box 0 190 480 82 0;Box 0 190 480 1 5;Label 8 200 'ARRIVALS' 9;Label 8 216 'Freighter inbound. Dock clerk has the next slot.' 6;Label 8 232 '> Check arrival board' 6;Label 8 256 'U/D options  X do  TRI ship' 7
$out=Join-Path $root 'outputs/arrivals-native-authored.png';$bmp.Save($out,[Drawing.Imaging.ImageFormat]::Png)
$contrast=[Drawing.Bitmap]::new(480,272);$contrastMap=@{0=0;1=0;2=2;3=2;4=6;5=6;6=6;7=7;8=8;9=9;10=10}
for($yy=0;$yy-lt272;$yy++){for($xx=0;$xx-lt480;$xx++){ $argb=$bmp.GetPixel($xx,$yy).ToArgb();$idx=0;for($pi=0;$pi-lt$palette.Count;$pi++){if($argb-eq$palette[$pi].ToArgb()){$idx=$pi;break}};$contrast.SetPixel($xx,$yy,$palette[$contrastMap[$idx]])}}
$contrast.Save((Join-Path $root 'outputs/arrivals-native-authored-contrast.png'),[Drawing.Imaging.ImageFormat]::Png);$contrast.Dispose()
$packed=[Collections.Generic.List[byte]]::new();for($y=20;$y -lt 188;$y++){for($x=6;$x -lt 346;$x+=2){$ia=0;$ib=0;$ca=$bmp.GetPixel($x,$y).ToArgb();$cb=$bmp.GetPixel($x+1,$y).ToArgb();for($i=0;$i -lt $palette.Count;$i++){if($ca -eq $palette[$i].ToArgb()){$ia=$i};if($cb -eq $palette[$i].ToArgb()){$ib=$i}};$packed.Add([byte](($ia -shl 4)-bor$ib))}}
$header=Join-Path $root 'src/generated/station-arrivals-authored-pixels.h';$writer=[IO.StreamWriter]::new($header,$false,[Text.UTF8Encoding]::new($false));try{$writer.WriteLine('/* Directly authored at native480x272; packed340x168 room indices. */');$writer.WriteLine('static const unsigned char station_arrivals_authored_pixels[28560]={');for($i=0;$i -lt $packed.Count;$i+=24){$last=[Math]::Min($i+23,$packed.Count-1);$writer.WriteLine(' '+(($packed[$i..$last]|ForEach-Object {'0x{0:x2}' -f $_})-join ',')+',')};$writer.WriteLine('};')}finally{$writer.Dispose()}
$g.Dispose();$bmp.Dispose();Write-Output 'Direct native authoring complete:480x272,11colour,room packed28560bytes'

