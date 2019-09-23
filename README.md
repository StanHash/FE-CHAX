# Stan's CHax for FE8U

GBAFE hacks (in C!) and `EA`/`make` template. See [FE8UASMHax](https://github.com/StanHash/FE8UASMHax) for older stuff (there's still a few things left there that I haven't ported to here yet).

## how2

- have [devkitARM](https://devkitpro.org/wiki/Getting_Started) installed, and the `DEVKITARM` env variable set.
- have `make` and some version of bash (if you're under Windows, devkitPro comes with msys bundled, which includes those among other things) in you path.
- have [python 3](https://www.python.org/) installed and in your path. (the makefile *should* be able to find the correct python executable name. See [`Tools.mak`](./Tools.mak)).
- have the following tools in the `Tools` folder, with the correct name (see [`Tools.mak`](./Tools.mak) in case names don't match):
  - [`EventAssembler/Core` (with the rest of the EA installation of course, including Tools)](https://feuniverse.us/t/event-assembler/1749?u=stanh)
    - There's no real need for a custom EA build anymore.
    - Using the legacy EA Core (aka not [`ColorzCore`](https://github.com/FireEmblemUniverse/ColorzCore)) is still required for some bits (symbol assignment is used in some places).
  - [`ea-dep`](https://github.com/StanHash/ea-dep) (used for dependency generation).
  - [`lyn`](https://github.com/StanHash/lyn/releases) (used for all wizardry and songs)
  - [YamaArashi's (?) `gbagfx`](https://github.com/pret/pokeruby-tools) (used for lz77 compression).
- run your shell and go to this folder
  - you can `make` or `make hack` to build the ROM (requires `FE8U.gba` in the directory)
  - If you want to make a speicifc hack, simply include its main installer from `Main.event` or `Wizardry/Wizardry.event` and `make` again.
  - you can `make SomeFile.[o|asm|dmp|lyn.event]` to make a specific file (`asm` files are generated from C).
  - run `make clean` to clean build outputs.
- hf :)

**Note**: This repository makes use of the git submodule facilities. If you didn't clone the repository using `git clone --recursive`, then you will need to `git submodule update --init --recursive` before being able to build this.

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
