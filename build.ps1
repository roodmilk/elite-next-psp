param(
    [string]$Toolchain = "$PSScriptRoot/../../work/toolchain",
    [string]$BuildDirectory = "$PSScriptRoot/build",
    [string]$OutputPath = "$PSScriptRoot/EBOOT.PBP"
)
$ErrorActionPreference = 'Stop'
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
$portObjects = @()
foreach ($source in @('game','ships','main')) {
    $portObject = "$portBuild/$source.o"
    Run-Tool 'psp-gcc' @('-std=gnu11','-O2','-G0','-Wall','-Wextra',"-I$portSdk/include",'-D_PSP_FW_VERSION=600','-c',"$PSScriptRoot/src/$source.c",'-o',$portObject)
    $portObjects += $portObject
}
Run-Tool 'psp-gcc' (@('-G0',"-L$portSdk/lib","-specs=$portSdk/lib/prxspecs","-Wl,-q,-T$portSdk/lib/linkfile.prx",'-Wl,-zmax-page-size=128') + $portObjects + @("$portSdk/lib/prxexports.o",'-lpspdebug','-lpspdisplay','-lpspge','-lpspctrl','-lpsppower','-lpsprtc','-lpspaudio','-lpspmp3','-lpsputility','-lvorbisfile','-lvorbis','-logg','-lm','-o',"$portBuild/elite-a.elf"))
Run-Tool 'psp-fixup-imports' @("$portBuild/elite-a.elf")
Run-Tool 'psp-prxgen' @("$portBuild/elite-a.elf","$portBuild/elite-a.prx")
Run-Tool 'mksfoex' @('-d','MEMSIZE=0','ELITE: NEXT 2.5.252',"$portBuild/PARAM.SFO")
Run-Tool 'pack-pbp' @($portOutput,"$portBuild/PARAM.SFO", "$PSScriptRoot/assets/xmb/ICON0.PNG", 'NULL', 'NULL', "$PSScriptRoot/assets/xmb/PIC1.PNG", 'NULL', "$portBuild/elite-a.prx", 'NULL')
Write-Output "Built $portOutput"

