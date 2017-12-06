@echo off

@rem Definitions
set DEVKITARM=C:\devkitPro\devkitARM\bin\

set as="%DEVKITARM%arm-none-eabi-as"
set gcc="%DEVKITARM%arm-none-eabi-gcc"

set INCLUDEPATH="%~dp0libgbafe"

@rem First, we assemble all *.s files to elves
for /R %%F in (*.s) do (
	%as% -g -mcpu=arm7tdmi -mthumb-interwork "%%~F" -o "%%~dpnF.s.elf"
)

@rem Next, we compile all *.c files to elves
for /R %%F in (*.c) do (
	%gcc% -ffast-math -fomit-frame-pointer -mcpu=arm7tdmi -mtune=arm7tdmi -Wall -Os -I %INCLUDEPATH% -mthumb -c "%%~F" -o "%%~dpnF.c.elf"
	%gcc% -ffast-math -fomit-frame-pointer -mcpu=arm7tdmi -mtune=arm7tdmi -Wall -Os -I %INCLUDEPATH% -mthumb -S "%%~F" -o "%%~dpnF.gen.s"
)

pause > nul
