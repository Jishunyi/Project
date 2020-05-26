SET PATH=D:\MDK5\ARM\ARMCC\Bin;D:\Python3\Scripts\;D:\Python3\;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Windows\System32\OpenSSH\;C:\Program Files (x86)\NVIDIA Corporation\PhysX\Common;D:\Git\Git\cmd;C:\Users\200422013\AppData\Local\Microsoft\WindowsApps;D:\Mingw\bin;D:\VSCode\Microsoft VS Code\bin
SET ARMCC5_ASMOPT=--diag_suppress=9931
SET ARMCC5_CCOPT=--diag_suppress=9931
SET ARMCC5_LINKOPT=--diag_suppress=9931
SET CPU_TYPE=LPC11C14FBD48/301
SET CPU_VENDOR=NXP
SET UV2_TARGET=LPC11C14_APP
SET CPU_CLOCK=0x00B71B00
"D:\MDK5\ARM\ARMCC\Bin\ArmCC" --Via ".\projectoutput\main.__i"
"D:\MDK5\ARM\ARMCC\Bin\ArmCC" --Via ".\projectoutput\logic.__i"
"D:\MDK5\ARM\ARMCC\Bin\ArmCC" --Via ".\projectoutput\user.__i"
"D:\MDK5\ARM\ARMCC\Bin\ArmCC" --Via ".\projectoutput\drviouart.__i"
"D:\MDK5\ARM\ARMCC\Bin\ArmCC" --Via ".\projectoutput\drvgpio.__i"
"D:\MDK5\ARM\ARMCC\Bin\ArmCC" --Via ".\projectoutput\drvtimer.__i"
"D:\MDK5\ARM\ARMCC\Bin\ArmCC" --Via ".\projectoutput\drvuart.__i"
"D:\MDK5\ARM\ARMCC\Bin\ArmCC" --Via ".\projectoutput\drvsysclk.__i"
"D:\MDK5\ARM\ARMCC\Bin\ArmCC" --Via ".\projectoutput\drvflash.__i"
"D:\MDK5\ARM\ARMCC\Bin\ArmCC" --Via ".\projectoutput\drvcan.__i"
"D:\MDK5\ARM\ARMCC\Bin\ArmCC" --Via ".\projectoutput\core_cm0.__i"
"D:\MDK5\ARM\ARMCC\Bin\ArmCC" --Via ".\projectoutput\system_lpc11xx.__i"
"D:\MDK5\ARM\ARMCC\Bin\ArmAsm" --Via ".\projectoutput\startup_lpc11xx._ia"
"D:\MDK5\ARM\ARMCC\Bin\ArmLink" --Via ".\ProjectOutput\MCU_C14.lnp"
"D:\MDK5\ARM\ARMCC\Bin\fromelf.exe" ".\ProjectOutput\MCU_C14.axf" --i32combined --output ".\ProjectOutput\MCU_C14.hex"
fromelf.exe --bin --output "C:\Users\200422013\Desktop\海外蒂森\LPC11C14_APP\Keil\ProjectOutput\MCU_C14.bin" "C:\Users\200422013\Desktop\海外蒂森\LPC11C14_APP\Keil\ProjectOutput\MCU_C14.axf"
