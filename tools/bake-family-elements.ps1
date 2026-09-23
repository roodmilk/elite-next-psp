param(
  [string]$Source = (Join-Path $PSScriptRoot '..\assets\source\station-art\family-references\family-element-atlas-source.png'),
  [string]$Output = (Join-Path $PSScriptRoot '..\assets\preview\reusable-element-kit\family-elements')
)

$ErrorActionPreference = 'Stop'
Add-Type -AssemblyName System.Drawing

New-Item -ItemType Directory -Force $Output | Out-Null
$palette = @('080d18','151c27','293646','5a604c','8b4b37','c18b4d','e5d2a3','9b9aa5','55d4d4','f0b45b','c85a4b') |
  ForEach-Object { [Drawing.ColorTranslator]::FromHtml('#' + $_) }

$targets = @(
  @{ id='prospector-ore-wall'; size=@(92,72); crop=@(35,20,245,350) },
  @{ id='prospector-assay-bench'; size=@(74,42); crop=@(310,95,365,290) },
  @{ id='prospector-cargo-rack'; size=@(52,64); crop=@(720,40,245,350) },
  @{ id='prospector-lander-window'; size=@(82,52); crop=@(1035,35,465,360) },
  @{ id='prospector-standing-sample-case'; size=@(18,48); crop=@(30,365,220,270) },
  @{ id='prospector-seated-miner'; size=@(18,38); crop=@(335,420,320,240) },
  @{ id='research-specimen-cabinet'; size=@(64,86); crop=@(35,580,245,400) },
  @{ id='research-survey-console'; size=@(70,44); crop=@(360,650,430,320) },
  @{ id='research-telescope'; size=@(58,58); crop=@(855,630,180,330) },
  @{ id='research-orbit-window'; size=@(104,64); crop=@(1050,500,450,480) },
  @{ id='research-standing-researcher'; size=@(18,48); crop=@(240,640,150,300) },
  @{ id='research-seated-researcher'; size=@(22,40); crop=@(430,650,330,320) }
)

function Nearest-Palette([Drawing.Color]$c, [bool]$Contrast) {
  if ($c.A -lt 32) { return [Drawing.Color]::Transparent }
  $best = $palette[0]; $bestD = [double]::MaxValue
  foreach ($p in $palette) {
    $r = [int]$c.R; $g = [int]$c.G; $b = [int]$c.B
    if ($Contrast) { $r=[Math]::Min(255,[int]($r*1.18)); $g=[Math]::Min(255,[int]($g*1.18)); $b=[Math]::Min(255,[int]($b*1.18)) }
    $dr=$r-$p.R; $dg=$g-$p.G; $db=$b-$p.B; $d=($dr*$dr)+($dg*$dg)+($db*$db)
    if ($d -lt $bestD) { $bestD=$d; $best=$p }
  }
  return [Drawing.Color]::FromArgb(255,$best.R,$best.G,$best.B)
}

function Crop-Fit([Drawing.Bitmap]$src, [int[]]$crop, [int]$w, [int]$h, [bool]$Contrast) {
  $cell = [Drawing.Bitmap]::new($crop[2],$crop[3],[Drawing.Imaging.PixelFormat]::Format32bppArgb)
  $cg=[Drawing.Graphics]::FromImage($cell); $cg.Clear([Drawing.Color]::Transparent)
  $cg.InterpolationMode=[Drawing.Drawing2D.InterpolationMode]::NearestNeighbor
  $cg.PixelOffsetMode=[Drawing.Drawing2D.PixelOffsetMode]::Half
  $cg.DrawImage($src,[Drawing.Rectangle]::new(0,0,$crop[2],$crop[3]),$crop[0],$crop[1],$crop[2],$crop[3],[Drawing.GraphicsUnit]::Pixel)
  $out=[Drawing.Bitmap]::new($w,$h,[Drawing.Imaging.PixelFormat]::Format32bppArgb)
  $og=[Drawing.Graphics]::FromImage($out); $og.Clear([Drawing.Color]::Transparent)
  $og.InterpolationMode=[Drawing.Drawing2D.InterpolationMode]::NearestNeighbor
  $og.PixelOffsetMode=[Drawing.Drawing2D.PixelOffsetMode]::Half
  $scale=[Math]::Min($w/$cell.Width,$h/$cell.Height)
  $dw=[Math]::Max(1,[int]($cell.Width*$scale)); $dh=[Math]::Max(1,[int]($cell.Height*$scale))
  $dx=[int](($w-$dw)/2); $dy=[int](($h-$dh)/2)
  $og.DrawImage($cell,[Drawing.Rectangle]::new($dx,$dy,$dw,$dh),0,0,$cell.Width,$cell.Height,[Drawing.GraphicsUnit]::Pixel)
  for($y=0;$y -lt $h;$y++) { for($x=0;$x -lt $w;$x++) { $out.SetPixel($x,$y,(Nearest-Palette $out.GetPixel($x,$y) $Contrast)) } }
  $og.Dispose(); $cg.Dispose(); $cell.Dispose(); return $out
}

$src=[Drawing.Bitmap]::new($Source)
$manifest=@()
foreach($t in $targets) {
  foreach($contrast in @($false,$true)) {
    $bmp=Crop-Fit $src $t.crop $t.size[0] $t.size[1] $contrast
    $suffix=if($contrast){'contrast'}else{'normal'}
    $path=Join-Path $Output ("$($t.id)-$suffix.png")
    $bmp.Save($path,[Drawing.Imaging.ImageFormat]::Png)
    $manifest += [ordered]@{id=$t.id;variant=$suffix;width=$t.size[0];height=$t.size[1];packed_4bit_bytes=[int](($t.size[0]*$t.size[1]+1)/2);source='family-element-atlas-source.png';runtime_ready=$false}
    $bmp.Dispose()
  }
}
$manifest | ConvertTo-Json -Depth 4 | Set-Content (Join-Path $Output 'MANIFEST.json') -Encoding UTF8
$src.Dispose()
Write-Output ("Baked {0} elements x 2 variants to {1}" -f $targets.Count,$Output)
