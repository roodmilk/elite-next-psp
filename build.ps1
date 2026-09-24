param([string]$Toolchain = "$PSScriptRoot/../../work/toolchain")
$ErrorActionPreference = 'Stop'
$portToolchain = (Resolve-Path -LiteralPath $Toolchain).Path.Replace('\','/')
$portSdk = "$portToolchain/psp/sdk"
$env:PATH = "$portToolchain/bin;" + $env:PATH
$env:PSPDEV = $portToolchain
$portBuild = "$PSScriptRoot/build"
New-Item -ItemType Directory -Force $portBuild | Out-Null
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
Run-Tool 'psp-gcc' (@('-G0',"-L$portSdk/lib","-specs=$portSdk/lib/prxspecs","-Wl,-q,-T$portSdk/lib/linkfile.prx",'-Wl,-zmax-page-size=128') + $portObjects + @("$portSdk/lib/prxexports.o",'-lpspdebug','-lpspdisplay','-lpspge','-lpspctrl','-lpsppower','-lpsprtc','-lpspaudio','-lpspmp3','-lpsputility','-lm','-o',"$portBuild/elite-a.elf"))
Run-Tool 'psp-fixup-imports' @("$portBuild/elite-a.elf")
Run-Tool 'psp-prxgen' @("$portBuild/elite-a.elf","$portBuild/elite-a.prx")
Run-Tool 'mksfoex' @('-d','MEMSIZE=0','ELITE: NEXT 2.5.80',"$portBuild/PARAM.SFO")
Run-Tool 'pack-pbp' @("$PSScriptRoot/EBOOT.PBP","$portBuild/PARAM.SFO",'NULL','NULL','NULL','NULL','NULL',"$portBuild/elite-a.prx",'NULL')
Write-Output "Built $PSScriptRoot/EBOOT.PBP"
