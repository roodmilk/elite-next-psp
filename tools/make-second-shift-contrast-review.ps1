param(
 [string]$Source='assets/second-shift-native.png',
 [string]$Output='assets/second-shift-native-contrast.png'
)
$ErrorActionPreference='Stop'
Add-Type -AssemblyName System.Drawing
$bmp=[Drawing.Bitmap]::new((Resolve-Path $Source).Path)
$map=@{
 '080d18'='080d18';'151c27'='080d18';'293646'='293646';'5a604c'='293646';
 '8b4b37'='e5d2a3';'c18b4d'='e5d2a3';'e5d2a3'='e5d2a3';'9b9aa5'='9b9aa5';
 '55d4d4'='55d4d4';'f0b45b'='f0b45b';'c85a4b'='c85a4b'
}
for($y=0;$y-lt$bmp.Height;$y++){for($x=0;$x-lt$bmp.Width;$x++){
 $c=$bmp.GetPixel($x,$y);$key=('{0:x2}{1:x2}{2:x2}'-f $c.R,$c.G,$c.B)
 if(!$map.ContainsKey($key)){throw "Unexpected non-palette pixel $key at $x,$y"}
 $bmp.SetPixel($x,$y,[Drawing.ColorTranslator]::FromHtml('#'+$map[$key]))
}}
New-Item -ItemType Directory -Force (Split-Path $Output) | Out-Null
$bmp.Save((Join-Path (Get-Location) $Output),[Drawing.Imaging.ImageFormat]::Png)
$bmp.Dispose()
Write-Output "Wrote native high-contrast review $Output"
