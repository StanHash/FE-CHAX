
# Stan's CHax for FE8U

GBAFE hacks (in C!) and `EA`/`make` template. See [FE8UASMHax](https://github.com/StanHash/FE8UASMHax) for older stuff (there's still a few things left there that I haven't ported to here yet).

## how2

You need to be on some Linux (Windows users: consider using [WSL](https://docs.microsoft.com/en-us/windows/wsl/install-win10)).

This repository makes use of the git submodule facilities. If you didn't clone the repository using `git clone --recursive`, then you will need to `git submodule update --init --recursive` before being able to build this.

- have [devkitARM](https://devkitpro.org/wiki/Getting_Started) installed, and the `DEVKITARM` env variable set.
- have [python 3](https://www.python.org/) installed and in your path. (the makefile *should* be able to find the correct python executable name. See [`tooks.mk`](./tooks.mk)).
- have built Tools/EventAssembler by running `Tools/EventAssembler/build.sh`. [See the attached README for details](https://github.com/StanHash/EventAssembler/blob/main/README.md).

Once all of that is done, you can navigate to this folder in your shell and do the following:

- `make` or `make hack` to build the ROM (requires `FE8U.gba` in the directory)
- If you want to make a speicifc hack, simply include its main installer from `Main.event` or `Wizardry/Wizardry.event` and `make` again.
- you can `make SomeFile.[o|asm|dmp|lyn.event]` to make a specific file (`asm` files are generated from C).
- run `make clean` to clean build outputs.

hf :)

## General `make`/EA guidelines

- **_Never_ use spaces in filenames/paths.** This breaks `make` horribly. This is one of the main reason I had to modify most of circles tools for them to work with this setup.
- Don't use `#incext`/`#inctext` unless you *really* need to. Use `#include`/`#incbin` with files generated from Makefile rules instead. This speeds up the build process tremendously.

## Big thankies

Kinda random but I feel like giving credit.

- circleseverywhere: Most tools in the `Tools/py` folder are based off his work. `_FE8EssentialFixes.event` is his composition (I think). Also lots of cool hax to base off mine. Also the original `MAKE HACK.cmd`. Also cool guy.
- Colorz: Maintains EA and implemented a lot of useful stuff. Also made `ParseFile` and `PortraitFormatter`, both of which are used here. Also useful hax. Also *Everything Assembler*. Also cool guy.
- Tequila: the Teq Doq is the best thing ever (I probably never would have gotten that far if not for that). Also lots of hax. Also cool guy. :duck:
- Teraspark, Leonarth, Kirb, Tiki/laqieer, ...: Cool and useful hax. Cool guys.
- Alusq: Demo/Test map. Cool guy.
- Nintenlord: He made the original EA. I never knew him but I assume he also was a cool guy.
- Zane: Doesn't have much to do with this but I love his work. Cool guy.
- Arch, Camdar: They prevent FEU from dying horribly (or at least from doing so for extented amounts of time). Cool guys.
- all of FEU: Cool guys.
