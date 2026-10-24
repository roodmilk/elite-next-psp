param([switch]$Retail)
$ErrorActionPreference='Stop';$root=Split-Path $PSScriptRoot;$bench=(Resolve-Path (Join-Path $root '../erl-implementation-candidate/outputs/magnetic-slice')).Path
$cases=if($Retail){@(@{flag='encounter-flight-capture';value='44';tag='port482-v3-lave';kind=1},@{flag='encounter-flight-capture';value='44';tag='port482-v3-capital';kind=2},@{flag='encounter-flight-capture';value='44';tag='port482-v3-relay';kind=3},@{flag='encounter-flight-capture';value='42';tag='port482-v3-cruise';kind=1},@{flag='encounter-flight-capture';value='43';tag='port482-v3-boost';kind=1})}else{@('port-ad-review','departure-hud-review','cockpit-hud-review','loadout-picker-review','dashboard-review','charm-review','final-menu','erl-quick-race-review')|ForEach-Object{@{flag=$_;value='1';tag='port482-v3';kind=1}}}
foreach($case in $cases){
 $eboot=Join-Path $root $(if($Retail){'build-port482-v3/EBOOT.PBP'}else{'build-port482-review-v3/EBOOT.PBP'})
 try{& (Join-Path $PSScriptRoot 'run-menu-prototypes.ps1') -Flag $case.flag -FlagValue $case.value -PortKind $case.kind -Eboot $eboot -Seconds 150 -RunTag $case.tag -Model 1 -Retail:$Retail}catch{if(!$Retail){throw};Write-Output $_.Exception.Message}
 $run=Join-Path $bench ('regression-'+$case.tag+'-diagnostic-'+$case.flag)
 foreach($child in @('emulator','music','EBOOT.PBP')){$target=Join-Path $run $child;if(Test-Path -LiteralPath $target){$resolved=(Resolve-Path -LiteralPath $target).Path;if(!$resolved.StartsWith($bench+[IO.Path]::DirectorySeparatorChar)){throw 'Cleanup outside owned benchmark workspace'};Remove-Item -LiteralPath $resolved -Force -Recurse}}
}
