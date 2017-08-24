nasm -f bin -i %~dp0src\ -o %~dp0bin\boot_mbr.bin %~dp0src\boot_mbr.asm
nasm -f bin -i %~dp0src\ -o %~dp0bin\boot.bin %~dp0src\boot.asm