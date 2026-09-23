param([string]$Source='assets/second-shift-native-v2-source.png')
$ErrorActionPreference='Stop'
& (Join-Path $PSScriptRoot 'bake-second-shift-native-v2.ps1') -Source $Source
if($LASTEXITCODE -ne 0){exit $LASTEXITCODE}
