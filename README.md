# Stan's CHax for FE8U

GBAFE hacks (in C!) and `EA`/`make` template. See [FE8UASMHax](https://github.com/StanHash/FE8UASMHax) for older stuff (there's a lot more hacks there than here).

## Do `make` & Event Assembler synergize

**Yes, [using this EA build](https://www.dropbox.com/s/moxk5tnerzhdkgl/EventAssembler-Stan.18.05.31.7z?dl=0)** ([Source is available at `FireEmblemUniverse/Event-Assembler` branch `stan-edits`](https://github.com/FireEmblemUniverse/Event-Assembler/tree/stan-edits)).

Note: [`ColorzCore`](https://github.com/FireEmblemUniverse/ColorzCore) is ***not*** compatible with this setup as I write these lines (july 4th 2018, 10:14AM BST).

## how2

- have [devkitARM](https://devkitpro.org/wiki/Getting_Started) installed, and the `DEVKITARM` env variable set.
- have `make` (and some kind of posix-compatible shell (if you're under Windows, devkitPro comes with msys bundled, which is more than enough) in you path.
- have [python 3](https://www.python.org/) installed and in your path. (the python executable is assumed to be called `python3`. Edit [`Tools.mak`](./Tools.mak) if you need to change that)
- have the following tools in the `Tools` folder, with the correct name (or you can edit [`Tools.mak`](./Tools.mak) in case names don't match):
  - [`lyn`](https://github.com/StanHash/lyn/releases)
  - [`EventAssembler/Core` (and the rest of the EA installation of course)](https://www.dropbox.com/s/moxk5tnerzhdkgl/EventAssembler-Stan.18.05.31.7z?dl=0)
    - **Using this custom build is _required_** (versions > 11.1.x would probably work, but those have yet to exist as I am writing this)
  - [Colorz's `PortraitFormatter`](http://feuniverse.us/t/ea-ea-formatting-suite/1714?u=stanh)
  - [Colorz's `ParseFile`](http://feuniverse.us/t/ea-ea-formatting-suite/1714?u=stanh)
  - [YamaArashi's (?) `gbagfx`](https://github.com/pret/pokeruby-tools) (unused right now)
- run your shell and go to this folder
  - you can `make` or `make hack` to build the demo ROM (requires `FE8U.gba` in the directory)
  - you can `make all` to make everything unconditionally.
  - you can `make SomeFile.[o|asm|dmp|lyn.event]` to make a specific file (`asm` files are generated from C).
  - run `make clean` when you're done.
- hf :)

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
