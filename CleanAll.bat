@echo off

for /R %%F in (*.elf) do (
	del %%F
)

for /R %%F in (*.lyn.event) do (
	del %%F
)

for /R %%F in (*.gen.s) do (
	del %%F
)

pause > nul
