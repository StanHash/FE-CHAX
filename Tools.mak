# making sure devkitARM exists and is set up
ifeq ($(strip $(DEVKITARM)),)
  $(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

# including devkitARM tool definitions
include $(DEVKITARM)/base_tools

# OS-specific tool calls (EA)
ifeq ($(OS),Windows_NT)
  export EA := Tools/EventAssembler/Core.exe
else
  export EA := mono Tools/EventAssembler/Core.exe
endif

# Making sure we are using python 3
ifeq ($(shell python -c 'import sys; print(int(sys.version_info[0] > 2))'),1)
  export PYTHON := python
else
  export PYTHON := python3
endif

# additional tools
export PARSEFILE         := Tools/ParseFile
export PORTRAITFORMATTER := Tools/PortraitFormatter
export LYN               := Tools/lyn
export GBAGFX            := Tools/gbagfx
export GENMUGS           := $(PYTHON) Tools/py/genmugs.py
export TEXTPROCESS       := $(PYTHON) Tools/py/textprocess.py
export C2EA              := $(PYTHON) Tools/py/NMM2CSV/c2ea.py
export TMX2EA            := $(PYTHON) Tools/py/tmx2ea/tmx2ea.py

PREPROCESS_MESSAGE = @echo "$(notdir $<) => $(notdir $@)"
