param([string]$Toolchain="$PSScriptRoot/../../../work/toolchain",[string]$SourceDir="")
$ErrorActionPreference='Stop'
$chartRoot=Split-Path $PSScriptRoot -Parent
$chartTools=(Resolve-Path -LiteralPath $Toolchain).Path.Replace('\','/')
$chartSdk="$chartTools/psp/sdk"
$env:PATH="$chartTools/bin;"+$env:PATH
$env:PSPDEV=$chartTools
$chartBuild=Join-Path $chartRoot ('outputs/chart-build-'+(Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
New-Item -ItemType Directory -Path $chartBuild | Out-Null
# Snapshot the source so concurrent work cannot replace objects or headers
# halfway through this candidate. The shared build/dist directories are idle.
$chartSource=if($SourceDir){(Resolve-Path -LiteralPath $SourceDir).Path}else{Join-Path $chartRoot 'src'}
Copy-Item -LiteralPath $chartSource -Destination (Join-Path $chartBuild 'src') -Recurse
function Chart-Tool([string]$Name,[string[]]$Arguments){
 & "$chartTools/bin/$Name.exe" @Arguments
 if($LASTEXITCODE -ne 0){throw "$Name failed ($LASTEXITCODE)"}
}
$chartObjects=@()
foreach($chartSource in @('game','ships','main')){
 $chartObject="$chartBuild/$chartSource.o"
 Chart-Tool 'psp-gcc' @('-std=gnu11','-O2','-G0','-Wall','-Wextra',"-I$chartSdk/include",'-D_PSP_FW_VERSION=600','-c',"$chartBuild/src/$chartSource.c",'-o',$chartObject)
 $chartObjects+=$chartObject
}
Chart-Tool 'psp-gcc' (@('-G0',"-L$chartSdk/lib","-specs=$chartSdk/lib/prxspecs","-Wl,-q,-T$chartSdk/lib/linkfile.prx",'-Wl,-zmax-page-size=128')+$chartObjects+@("$chartSdk/lib/prxexports.o",'-lpspdebug','-lpspdisplay','-lpspge','-lpspctrl','-lpsppower','-lpsprtc','-lpspaudio','-lpspmp3','-lpsputility','-lvorbisfile','-lvorbis','-logg','-lm','-o',"$chartBuild/elite-a.elf"))
Chart-Tool 'psp-fixup-imports' @("$chartBuild/elite-a.elf")
Chart-Tool 'psp-prxgen' @("$chartBuild/elite-a.elf","$chartBuild/elite-a.prx")
$chartVersion=(Get-Content -LiteralPath (Join-Path $chartRoot 'VERSION') -Raw).Trim()
Chart-Tool 'mksfoex' @('-d','MEMSIZE=0',"ELITE: NEXT $chartVersion","$chartBuild/PARAM.SFO")
Chart-Tool 'pack-pbp' @("$chartBuild/EBOOT.PBP","$chartBuild/PARAM.SFO","$chartRoot/assets/xmb/ICON0.PNG",'NULL','NULL',"$chartRoot/assets/xmb/PIC1.PNG",'NULL',"$chartBuild/elite-a.prx",'NULL')
Write-Output "Candidate: $chartBuild/EBOOT.PBP"
