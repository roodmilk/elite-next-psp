param([switch]$Retail)
$ErrorActionPreference='Stop';$root=Split-Path $PSScriptRoot;$bench=(Resolve-Path (Join-Path $root '../erl-implementation-candidate/outputs/magnetic-slice')).Path
$cases=if($Retail){@(@{flag='encounter-flight-capture';value='29';tag='guns485-hit';kind=1},@{flag='encounter-flight-capture';value='34';tag='guns485-switch';kind=1},@{flag='encounter-flight-capture';value='36';tag='guns485-busy';kind=1},@{flag='encounter-flight-capture';value='37';tag='guns485-station';kind=1},@{flag='encounter-flight-capture';value='32';tag='guns485-thirdperson';kind=1})}else{@('cockpit-hud-review','port-ad-review','departure-hud-review','loadout-picker-review','dashboard-review','charm-review','final-menu','erl-quick-race-review')|ForEach-Object{@{flag=$_;value='1';tag='guns485';kind=1}}}
foreach($case in $cases){
 $eboot=Join-Path $root $(if($Retail){'build-guns485/EBOOT.PBP'}else{'build-guns485-review/EBOOT.PBP'})
 try{& (Join-Path $PSScriptRoot 'run-menu-prototypes.ps1') -Flag $case.flag -FlagValue $case.value -PortKind $case.kind -CockpitShip 6 -CockpitMounts -Eboot $eboot -Seconds 150 -RunTag $case.tag -Model 1 -Retail:$Retail}catch{if(!$Retail){throw};Write-Output $_.Exception.Message}
 $run=Join-Path $bench ('regression-'+$case.tag+'-diagnostic-'+$case.flag)
 foreach($child in @('emulator','music','EBOOT.PBP')){$target=Join-Path $run $child;if(Test-Path -LiteralPath $target){$resolved=(Resolve-Path -LiteralPath $target).Path;if(!$resolved.StartsWith($bench+[IO.Path]::DirectorySeparatorChar)){throw 'Cleanup outside owned benchmark workspace'};Remove-Item -LiteralPath $resolved -Force -Recurse}}
}
