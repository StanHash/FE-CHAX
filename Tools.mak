# making sure devkitARM exists and is set up
ifeq ($(strip $(DEVKITARM)),)
  $(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

# including devkitARM tool definitions
include $(DEVKITARM)/base_tools

# OS-specific tool calls (EA)
ifeq ($(OS),Windows_NT)
  export EA := $(realpath .)/Tools/EventAssembler/Core.exe
else
  export EA := mono $(realpath .)/Tools/EventAssembler/Core.exe
endif

# Making sure we are using python 3
ifeq ($(shell python -c 'import sys; print(int(sys.version_info[0] > 2))'),1)
  export PYTHON3 := python
else
  export PYTHON3 := python3
endif

# additional tools
export PARSEFILE         := $(realpath .)/Tools/ParseFile
export PORTRAITFORMATTER := $(realpath .)/Tools/PortraitFormatter
export LYN               := $(realpath .)/Tools/lyn
export GBAGFX            := $(realpath .)/Tools/gbagfx
export GENMUGS           := $(PYTHON3) $(realpath .)/Tools/py/genmugs.py
export TEXTPROCESS       := $(PYTHON3) $(realpath .)/Tools/py/textprocess.py
export C2EA              := $(PYTHON3) $(realpath .)/Tools/py/NMM2CSV/c2ea.py
export TMX2EA            := $(PYTHON3) $(realpath .)/Tools/py/tmx2ea/tmx2ea.py

PREPROCESS_MESSAGE = @echo "$(notdir $<) => $(notdir $@)"
