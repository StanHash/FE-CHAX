# FE-CHAX

Headers, library and "hacks" for GBAFE; mostly written in C. See [FE8UASMHax](https://github.com/StanHash/FE8UASMHax) for more (older) stuff.

## about how `make` & Event Assembler synergize

They don't. Not really well at least. There's two main reasons for that:
- `Core.exe` requires the current directory to be the Event Assembler directory, which requires us to resort to some trickery when it comes to calling EA from [the Makefile](./Makefile): we're using a trampoline shell script that changes the working directory to EA's before calling Core (see below for an example of said script).
- EA doesn't provide us any easy way of generating `#include` (and `#incbin`) dependency rules, which is a problem since this means that `make` won't be able to detect needing to rebuild the target if any file that isn't the main file ([Main.event](./Main.event) in our case) was updated. We can bypass this limitation by simply calling `make clean && make`, but then what purpose do we even have using makefiles?
  - I'm currently working on a simple tool that will do just that: generate dependency rules for event files à la `gcc` with the `-M` option. This would solve the problem for `#include` & `#incbin` (which are the main directives we use that depend on external files). The `#incext` family of directives are still problematic but what can we do? I don't really see how to fix those without hard-coding argument interpreters for each commonly used tool (`Png2Dmp`, `ParseFile`, `compress`, `PortraitFormatter`...).

## how2

- have [devkitARM](https://devkitpro.org/wiki/Getting_Started/devkitARM) installed, and the `DEVKITARM` env variable set.
- have `make` (and some kind of posix-compatible shell (if you're under Windows, look into `<devkitPro>/msys/bin/`))
- have [`lyn`](https://github.com/StanHash/lyn/releases) & `ea.sh` (see below) in a new `bin` folder.
- run your shell and go to this folder
- run `make`
- should be done, use them files
- run `make clean` to clean built files

Sorry for the messiness of the whole process, this is kind of an experimental build structure, I'll be working on improving the whole thing in the future.

## `ea.sh` that I use (Linux)

```sh
#!/bin/sh

cd "$(dirname -- "$0")/EventAssembler" || exit 1
mono ./Core.exe "$@"
```

put that in a new `bin` subfolder, along with [`lyn`](https://github.com/StanHash/lyn/releases) and an `EventAssembler` folder containing latest [Event Assembler](http://feuniverse.us/t/event-assembler/1749?u=stanh).

(Also don't forget .NET/Mono to be able to run EA)

Under Windows, you probably can just omit `mono`, as .NET executables are considered as native Windows executables (I think), and can be called as they are. Requires testing as I don't have access to a Windows system right now.
