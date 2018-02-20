# FE-CHAX

Headers, library and "hacks" for GBAFE; mostly written in C. See [FE8UASMHax](https://github.com/StanHash/FE8UASMHax) for more (older) stuff.

## about how `make` & Event Assembler synergize

They don't. Not really well at least. There's two main reasons for that:

- `Core.exe` requires the current directory to be the Event Assembler directory, which requires us to resort to some trickery when it comes to calling EA from [the Makefile](./Makefile): we need to define files passed to EA through their *absolute* path, and then we're using a trampoline shell script that changes the working directory to EA's before calling Core (see below for an example of said script).
- EA doesn't provide us any easy way of generating `#include` (and `#incbin`) dependency rules, which is a problem since this means that `make` won't be able to detect needing to rebuild the target if any file that isn't the main file ([Main.event](./Main.event) in our case) was updated. We can bypass this limitation by simply calling `make clean && make` or by making the ROM a forced target, but then what purpose do we even have using makefiles?
  - `pea` fixes this issue now, as it does makefile dependency generation ala `gcc -M`.

## how2

- have [devkitARM](https://devkitpro.org/wiki/Getting_Started/devkitARM) installed, and the `DEVKITARM` env variable set.
- have `make` (and some kind of posix-compatible shell (if you're under Windows, look into `<devkitPro>/msys/bin/`))
- have [`lyn`](https://github.com/StanHash/lyn/releases), [`pea`](https://github.com/StanHash/pea-preprocessor/releases) & `ea` (see below) in a new `bin` folder.
- run your shell and go to this folder
  - you can `make` or `make rom` to build the demo ROM (requires `FE8_U.gba` in the directory)
  - you can `make all` to make everything unconditionally.
  - you can `make SomeFile.[o|asm|dmp|lyn.event]` to make a specific file (`asm` files are generated from C).
  - run `make clean` when you're done.
- Use the files in your project and hf :)

Sorry for the messiness of the whole process, this is kind of an experimental build structure, I'll be working on improving the whole thing in the future.

NOTE: if you are using `#incbin` or `#incext` anywhere in your buildfile, you need to have [`bin2ea`](https://github.com/StanHash/bin2ea) in the tools folder of EA (or another tool in another folder, see [the `pea` README](https://github.com/StanHash/pea-preprocessor/blob/master/README.md) for details)

## `ea` shell script that I use (Linux)

```sh
#!/bin/sh

cd "$(dirname -- "$0")/EventAssembler" || exit 1
mono ./Core.exe "$@"
```

put that in a new `bin` subfolder, along with [`lyn`](https://github.com/StanHash/lyn/releases), [`pea`](https://github.com/StanHash/pea-preprocessor/releases) and an `EventAssembler` folder containing latest [Event Assembler](http://feuniverse.us/t/event-assembler/1749?u=stanh).

(Also don't forget .NET/Mono to be able to run EA)

Under Windows, you probably can just omit `mono`, as .NET executables are considered as native Windows executables.
