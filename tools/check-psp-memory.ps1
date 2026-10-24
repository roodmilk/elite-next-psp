param([Parameter(Mandatory=$true)][string]$Elf,[string]$Prx='', [switch]$Psp3000)
$ErrorActionPreference='Stop'
$memoryElf=[IO.File]::ReadAllBytes((Resolve-Path -LiteralPath $Elf))
if([BitConverter]::ToUInt32($memoryElf,0) -ne 0x464c457f -or $memoryElf[4] -ne 1 -or $memoryElf[5] -ne 1){throw 'Expected a 32-bit little-endian PSP ELF'}
$memoryOffset=[BitConverter]::ToUInt32($memoryElf,28)
$memoryStride=[BitConverter]::ToUInt16($memoryElf,42)
$memoryCount=[BitConverter]::ToUInt16($memoryElf,44)
$memoryEnd=0
for($memoryIndex=0;$memoryIndex -lt $memoryCount;$memoryIndex++){
 $memoryHeader=$memoryOffset+$memoryIndex*$memoryStride
 if([BitConverter]::ToUInt32($memoryElf,$memoryHeader) -eq 1){
  $memorySegmentEnd=[BitConverter]::ToUInt32($memoryElf,$memoryHeader+8)+[BitConverter]::ToUInt32($memoryElf,$memoryHeader+20)
  $memoryEnd=[Math]::Max($memoryEnd,$memorySegmentEnd)
 }
}
# The retail main declares a 7 MiB heap and 1 MiB stack. Keep 3 MiB free
# for the firmware's codec modules, worker stacks and allocation overhead.
$memoryPartition=if($Psp3000){52MB}else{24MB}
$memoryHeap=if($Psp3000){12MB}else{7MB}
$memoryHeadroom=$memoryPartition-$memoryEnd-$memoryHeap-1MB
if($memoryHeadroom -lt 3MB){throw "Retail memory budget exceeded: only $memoryHeadroom bytes remain before codec/thread overhead"}
if($Prx -and ($memoryEnd+(Get-Item -LiteralPath $Prx).Length) -gt $memoryPartition){throw 'PRX staging plus loaded image exceeds the original PSP user partition'}
Write-Output "PASS retail PSP memory budget: image=$memoryEnd heap=$memoryHeap main_stack=1048576 headroom=$memoryHeadroom psp3000=$Psp3000"
