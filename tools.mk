ifeq ($(OS),Windows_NT)
  EXE := .exe
else
  EXE :=
endif

TOOLCHAIN ?= $(DEVKITARM)

ifneq (,$(TOOLCHAIN))
  export PATH := $(TOOLCHAIN)/bin:$(PATH)
endif

PREFIX := arm-none-eabi-

CC := $(PREFIX)gcc
AS := $(PREFIX)as
OBJCOPY := $(PREFIX)objcopy

# Making sure we are using python 3
ifeq ($(shell python -c 'import sys; print(int(sys.version_info[0] > 2))'),1)
  export PYTHON3 := python
else
  export PYTHON3 := python3
endif

EA := $(realpath .)/Tools/EventAssembler/ColorzCore$(EXE)

# additional tools
export PARSEFILE         := $(realpath .)/Tools/EventAssembler/Tools/ParseFile$(EXE)
export PORTRAITFORMATTER := $(realpath .)/Tools/EventAssembler/Tools/PortraitFormatter$(EXE)
export PNG2DMP           := $(realpath .)/Tools/EventAssembler/Tools/Png2Dmp$(EXE)
export COMPRESS          := $(realpath .)/Tools/EventAssembler/Tools/compress$(EXE)
export LYN               := $(realpath .)/Tools/EventAssembler/Tools/lyn$(EXE)
export EADEP             := $(realpath .)/Tools/EventAssembler/ea-dep$(EXE)

export PORTRAIT_PROCESS  := $(PYTHON3) $(realpath .)/Tools/PyTools/portrait-process.py
export TEXT_PROCESS      := $(PYTHON3) $(realpath .)/Tools/PyTools/text-process-classic.py
export C2EA              := $(PYTHON3) $(realpath .)/Tools/PyTools/NMM2CSV/c2ea.py
export TMX1EA            := $(PYTHON3) $(realpath .)/Tools/PyTools/tmx1ea.py

NOTIFY_PROCESS = @echo "$(notdir $<) => $(notdir $@)"
