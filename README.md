# FE-CHAX

See [FE8UASMHax](https://github.com/StanHash/FE8UASMHax) for more (probably older) related hax.

## how2

- Make sure you have [devkitARM](https://devkitpro.org/wiki/Getting_Started/devkitARM) installed (if your install location differs from default, do check and modify `AssembleAll.bat` to make it consistent with your installation).
- Make a new `bin` folder and put [`lyn.exe`](https://github.com/StanHash/lyn/releases) in that
- Run `AssembleAll.bat`, and then `LynAll.bat` to generated binaries & event files required for installation.
- Use the generated files in ur project (aka the fun part)
- Run `CleanAll.bat` when you're done (or don't I won't judge)
