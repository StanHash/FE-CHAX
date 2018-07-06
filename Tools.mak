# making sure devkitARM exists and is set up
ifeq ($(strip $(DEVKITARM)),)
	$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

# including devkitARM tool definitions
include $(DEVKITARM)/base_tools

# python executable name
export PYTHON := python3

# additional tools
export EA                := Tools/EventAssembler/Core
export PARSEFILE         := Tools/ParseFile
export PORTRAITFORMATTER := Tools/PortraitFormatter
export LYN               := Tools/lyn
export GBAGFX            := Tools/gbagfx
export GENMUGS           := $(PYTHON) Tools/py/genmugs.py
export TEXTPROCESS       := $(PYTHON) Tools/py/textprocess.py
export C2EA              := $(PYTHON) Tools/py/c2ea.py
export TMX2EA            := $(PYTHON) Tools/py/tmx2ea/tmx2ea.py

PREPROCESS_MESSAGE = @echo "$(notdir $<) => $(notdir $@)"
