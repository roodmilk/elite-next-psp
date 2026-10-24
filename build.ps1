# Release 2.5.488: permanent gun housings and immediate primary fire; retail excludes embedded review code.
param(
    [string]$Toolchain = "$PSScriptRoot/../../work/toolchain",
    [string]$BuildDirectory = "$PSScriptRoot/build",
    [string]$OutputPath = "$PSScriptRoot/EBOOT.PBP",
    [string]$GameVersion = '',
    [switch]$EnableReview,
    [switch]$ReviewExtendedMemory,
    [switch]$Psp3000,
    [switch]$EncounterReview
)
$ErrorActionPreference = 'Stop'
if ($EnableReview -or $EncounterReview) {
    if (!$PSBoundParameters.ContainsKey('OutputPath')) { $OutputPath = "$PSScriptRoot/outputs/review-build/EBOOT.PBP" }
    if (!$PSBoundParameters.ContainsKey('BuildDirectory')) { $BuildDirectory = "$PSScriptRoot/outputs/review-build" }
}
$portToolchain = (Resolve-Path -LiteralPath $Toolchain).Path.Replace('\','/')
$portSdk = "$portToolchain/psp/sdk"
$env:PATH = "$portToolchain/bin;" + $env:PATH
$env:PSPDEV = $portToolchain
$portBuild = [System.IO.Path]::GetFullPath($BuildDirectory)
$portOutput = [System.IO.Path]::GetFullPath($OutputPath)
New-Item -ItemType Directory -Force $portBuild | Out-Null
New-Item -ItemType Directory -Force ([System.IO.Path]::GetDirectoryName($portOutput)) | Out-Null
function Run-Tool([string]$Tool, [string[]]$ToolArgs) {
    & "$portToolchain/bin/$Tool.exe" @ToolArgs
    if ($LASTEXITCODE -ne 0) { throw "$Tool failed with exit code $LASTEXITCODE" }
}
# Never ship the embedded automated QA suite on the physical PSP.
[string[]]$portReviewFlags = @()
if ($EnableReview) { $portReviewFlags += '-DELITE_ENABLE_REVIEW=1' }
if ($EncounterReview) { $portReviewFlags += '-DELITE_ENCOUNTER_REVIEW=1' }
if ($Psp3000) { $portReviewFlags += '-DELITE_PSP3000=1' }
$portObjects = @()
foreach ($source in @('game','ships','main')) {
    $portObject = "$portBuild/$source.o"
    Run-Tool 'psp-gcc' (@('-std=gnu11','-O3','-flto','-fno-math-errno','-G0','-Wall','-Wextra',"-I$portSdk/include",'-D_PSP_FW_VERSION=600','-c',"$PSScriptRoot/src/$source.c",'-o',$portObject) + $portReviewFlags)
    $portObjects += $portObject
}
# Bound the host compiler's LTO heap; this does not change PSP optimisation.
Run-Tool 'psp-gcc' (@('-O3','-flto','--param=ggc-min-expand=20','--param=ggc-min-heapsize=32768','-fno-math-errno','-G0',"-L$portSdk/lib","-specs=$portSdk/lib/prxspecs","-Wl,-q,-T$portSdk/lib/linkfile.prx",'-Wl,-zmax-page-size=128') + $portObjects + @("$portSdk/lib/prxexports.o",'-lpspdebug','-lpspgu','-lpspdisplay','-lpspge','-lpspctrl','-lpsppower','-lpsprtc','-lpspaudio','-lpspmp3','-lpspjpeg','-lpsputility','-lpspnet_apctl','-lpspnet_inet','-lpspnet_adhocctl','-lpspnet_adhoc','-lpspnet','-lpspwlan','-lvorbisfile','-lvorbis','-logg','-lm','-o',"$portBuild/elite-a.elf"))
Run-Tool 'psp-fixup-imports' @("$portBuild/elite-a.elf")
Run-Tool 'psp-prxgen' @("$portBuild/elite-a.elf","$portBuild/elite-a.prx")
if (!$EnableReview) { & "$PSScriptRoot/tools/check-psp-memory.ps1" -Elf "$portBuild/elite-a.elf" -Prx "$portBuild/elite-a.prx" -Psp3000:$Psp3000 }
if (!$GameVersion) { $GameVersion = [IO.File]::ReadAllText((Join-Path $PSScriptRoot 'VERSION')).Trim() }
if ($GameVersion -notmatch '^\d+\.\d+\.\d+$') { throw 'Invalid build version' }
if ($ReviewExtendedMemory -and !$EnableReview) { throw 'Extended memory is for diagnostics only' }
$portMemory = if ($ReviewExtendedMemory -or $Psp3000) { 'MEMSIZE=1' } else { 'MEMSIZE=0' }
Run-Tool 'mksfoex' @('-d',$portMemory,"ELITE: NEXT $GameVersion","$portBuild/PARAM.SFO")
$introFilmPath = Join-Path $PSScriptRoot 'assets/intro/cinematic.enm'
$introFilmPsar = if (Test-Path -LiteralPath $introFilmPath) { $introFilmPath } else { 'NULL' }
Run-Tool 'pack-pbp' @($portOutput,"$portBuild/PARAM.SFO", "$PSScriptRoot/assets/xmb/ICON0.PNG", 'NULL', 'NULL', "$PSScriptRoot/assets/xmb/PIC1.PNG", 'NULL', "$portBuild/elite-a.prx", $introFilmPsar)



Write-Output "Built $portOutput"






