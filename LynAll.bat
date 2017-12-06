@echo off

set lyn="%~dp0bin\lyn.exe"
set lib="%~dp0libgbafe\fe8u.s.elf"

for /R %%F in (*.c.elf) do (
	%lyn% "%%~F" %lib% > "%%~dpnF.lyn.event"
)

for /R %%F in (*.s.elf) do (
	%lyn% "%%~F" %lib% > "%%~dpnF.lyn.event"
)

pause > nul
