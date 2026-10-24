param([switch]$Retail)
$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot
$bench=(Resolve-Path (Join-Path $root '../erl-implementation-candidate/outputs/magnetic-slice')).Path
$cases=if($Retail){@(@{flag='encounter-flight-capture';value='42';tag='load480-v4-cruise'},@{flag='encounter-flight-capture';value='43';tag='load480-v4-boost'},@{flag='encounter-flight-capture';value='37';tag='load480-v4-station'},@{flag='encounter-flight-capture';value='19';tag='load480-v4-computers'})}else{@('loadout-picker-review','cockpit-hud-review','dashboard-review','charm-review','final-menu','erl-quick-race-review')|ForEach-Object{@{flag=$_;value='1';tag='load480-v4'}}}
foreach($case in $cases){
 $eboot=Join-Path $root $(if($Retail){'build-loadout480-v4/EBOOT.PBP'}else{'build-loadout480-review-v4/EBOOT.PBP'})
 try{& (Join-Path $PSScriptRoot 'run-menu-prototypes.ps1') -Flag $case.flag -FlagValue $case.value -Eboot $eboot -Seconds 90 -RunTag $case.tag -Model 1 -Retail:$Retail}catch{if(!$Retail){throw};Write-Output $_.Exception.Message}
 $run=Join-Path $bench ('regression-'+$case.tag+'-diagnostic-'+$case.flag)
 foreach($child in @('emulator','music','EBOOT.PBP')){
  $target=Join-Path $run $child
  if(Test-Path -LiteralPath $target){$resolved=(Resolve-Path -LiteralPath $target).Path;if(!$resolved.StartsWith($bench+[IO.Path]::DirectorySeparatorChar)){throw 'Cleanup escaped owned benchmark directory'};Remove-Item -LiteralPath $resolved -Force -Recurse}
 }
}

