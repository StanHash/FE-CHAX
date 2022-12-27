# nat5/Stan's C wizardry for FE8-US ("C HAX")

GBAFE wizardry (in C!) and `EA`/`make` demo. See [FE8UASMHax][fe8u_asm_hax] for way older things (there's still a few things left there that I haven't ported to here yet).

**Wizardry** is the term I use to identify blobs of "ASM" code patches (which are made by "Wizards"). Traditionally those were called "ASM Hacks" or "ASM Patches" but with the possibility of using C rather than ASM, these names became a bit obsolete. Another popular name for these is "Engine Hacks".

See [releases][chax_releases] for pre-built ("ready to include") versions of wizardry featured here.

Everything in here (that I made) is released under [CC0][cc0] unless specified otherwise.

## Building

You probably need to be on some Linux (Windows users: consider using [WSL][wsl]).

This repository makes use of the git submodule facilities. If you didn't clone the repository using `git clone --recursive`, then you will need to `git submodule update --init --recursive` before being able to build this.

- Have a arm-none-eabi binutils+gcc toolchain. On Debian/Ubuntu you can simply `sudo apt install arm-none-eabi-gcc`. Otherwise, devkitARM works too.
- have [python 3][python] installed and in your path. (the makefile *should* be able to find the correct python executable name. See [`tooks.mk`](./tooks.mk)).
- have built Tools/EventAssembler by running `Tools/EventAssembler/build.sh`. [See the attached README for details][ea_readme].

Once all of that is done, you can navigate to this folder in your shell and do the following:

- `make` or `make hack` to build the ROM (requires `FE8U.gba` in the directory)
- If you want to make a speicifc hack, simply include its main installer from `Main.event` or `Wizardry/Wizardry.event` and `make` again.
- you can `make SomeFile.[o|asm|dmp|lyn.event]` to make a specific file (`asm` files are generated from C).
- run `make clean` to clean build outputs.

hf :)

## Thankies

I stole one of the test maps from Alusq.

Some of the wizardry in this repository (under Wizardry/3rdParty) was borrowed from circleseverywhere, Tequila, Kirb and Zane Avernathy.

Shout outs to circleseverywhere, Tequila, CrazyColorz5, Zane Avernathy, Leonarth, Tiki/laqieer and 7743. These are all wonderful and nice people from whom I have learned (and borrowed) so many things. I wouldn't be here without them.

Shout outs to the people of Fire Emblem Universe for being nice.

## Other things

- [**DOC**: GBAFE reverse-engineering notes][doc].
- [**fe6**: decompilation of Fire Emblem: The Binding Blade][fe6].
- [**fe6-wizardry**: wizardry for fe6][fe6_wizardry].
- [**fe7-stuff**: catchall for fe7-related things (wizardry, doc, projects, ...)][fe7_stuff].
- [**gbafe-eu-wizardry**: misc wizardry for fe7-eu1, fe7-eu2 and fe8-eu][eu_wizardry].
- [**SkillsLite**: an attempt to make a usable minimal alternative to the ubiquitous FEU skill system][skillslite].
- [**VBA-MAKE**][vba_make], [**FRIC**][fric], [**telefon-stan**][telefon_stan]: various other projects which feature wizardry not found elsewhere.

## Contact

You can find me over at the [Fire Emblem Universe Discord][feu_discord] under the handle `nat5#4387`.

[fe8u_asm_hax]: https://github.com/StanHash/FE8UASMHax
[chax_releases]: https://github.com/StanHash/FE-CHAX/releases
[cc0]: https://creativecommons.org/publicdomain/zero/1.0/
[wsl]: https://learn.microsoft.com/en-us/windows/wsl/install
[python]: https://www.python.org/
[ea_readme]: https://github.com/StanHash/EventAssembler/blob/main/README.md
[doc]: https://github.com/StanHash/DOC
[fe6]: https://github.com/StanHash/fe6
[fe6_wizardry]: https://github.com/StanHash/fe6-wizardry
[fe7_stuff]: https://github.com/StanHash/fe7-stuff
[eu_wizardry]: https://github.com/StanHash/gbafe-eu-wizardry
[skillslite]: https://github.com/StanHash/SkillsLite
[vba_make]: https://github.com/StanHash/VBA-MAKE
[fric]: https://github.com/StanHash/FRIC
[telefon_stan]: https://github.com/StanHash/telefon-stan
[feu_discord]: https://feuniverse.us/t/feu-discord-server/1480?u=stanh
