param(
    [string]$Manifest = "$PSScriptRoot/native-art-manifest.json"
)
$ErrorActionPreference = 'Stop'
$root = (Resolve-Path (Join-Path $PSScriptRoot '..')).Path
$data = Get-Content -Raw -LiteralPath $Manifest | ConvertFrom-Json

if ($data.screen.width -ne 480 -or $data.screen.height -ne 272) { throw 'Native screen must be exactly 480x272.' }
if ($data.screen.framebuffer_stride -ne 512) { throw 'PSP framebuffer stride must remain 512.' }
if ($data.format.runtime -ne 'ARGB1555' -or $data.format.sampling -ne 'nearest' -or $data.format.scale -ne 1) { throw 'Runtime art must use ARGB1555, nearest sampling, and 1x source scale.' }
if ($data.palette.Count -gt $data.format.max_palette_entries) { throw 'Palette exceeds the native art budget.' }

$seen = @{}
foreach ($scene in $data.scenes) {
    if ($seen.ContainsKey($scene.id)) { throw "Duplicate scene id: $($scene.id)" }
    $seen[$scene.id] = $true
    foreach ($field in @('source','native_review')) {
        $path = Join-Path $root $scene.$field
        if (!(Test-Path -LiteralPath $path)) { throw "Missing $field for $($scene.id): $($scene.$field)" }
    }
    if ([string]::IsNullOrWhiteSpace($scene.runtime_function)) { throw "Missing runtime function for $($scene.id)" }
    $l = $scene.layout
    if ($l.x -lt 0 -or $l.y -lt 0 -or $l.x + $l.width -gt 480 -or $l.y + $l.height -gt 272) { throw "Scene outside 480x272: $($scene.id)" }
    if ($l.safe_top -lt 0 -or $l.safe_bottom -gt 272 -or $l.safe_top -ge $l.safe_bottom) { throw "Invalid safe band: $($scene.id)" }
    if ($l.width % 1 -ne 0 -or $l.height % 1 -ne 0) { throw "Non-native dimension: $($scene.id)" }
    Write-Output ("PASS native-art {0} {1}x{2} at ({3},{4})" -f $scene.id,$l.width,$l.height,$l.x,$l.y)
}
Write-Output ("RESULT 0 failures — {0} native scenes validated" -f $data.scenes.Count)
