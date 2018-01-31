.SUFFIXES:
.PHONY: clean

# -----------
# MAIN CONFIG

# setting up main dir
# we want to use an absolute path because EA requires changing working directory
MAIN_DIR := $(realpath .)

# main files
MAIN       = $(MAIN_DIR)/Main.event
BASE_ROM   = $(MAIN_DIR)/FE8_U.gba
OUTPUT_ROM = $(MAIN_DIR)/OUT.gba

# -----------------------
# NON-EVENT BUILD OPTIONS

# making sure devkitARM exists and is set up
ifeq ($(strip $(DEVKITARM)),)
	$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

# including devkitARM tool definitions
# We aren't using devkitARM rules since we want to output things slightly differently (no dependancy things yet)
include $(DEVKITARM)/base_tools

# setting up additional path
export PATH := $(abspath .)/bin:$(PATH)

# setting up additional tools
# right now integrating EA is kind of a mess, so we use a trampoline shell script that changes the working directory
export EA  := ea.sh
export LYN := lyn

# CC/AS options

INCLUDE_DIRS := $(MAIN_DIR)/libgbafe
ARCH         := -mcpu=arm7tdmi -mthumb -mthumb-interwork

CCFLAGS      := -g -Wall -O3 -mtune=arm7tdmi -fomit-frame-pointer -ffast-math
ASFLAGS      := -g

# lyn options

LYN_LIBS := $(MAIN_DIR)/libgbafe/fe8u.s.o
LYNFLAGS := $(LYN_LIBS)

# ----------
# AUTOMATICS

# Generating AS/CC include directory flags from directory list
INCFLAGS := $(foreach dir, $(INCLUDE_DIRS), -I $(dir))

CCFLAGS += $(ARCH) $(INCFLAGS)
ASFLAGS += $(ARCH) $(INCFLAGS)

# Finding all C files
CFILES := $(shell find $(MAIN_DIR) -name '*.c')

# Finding all S files
SFILES := $(shell find $(MAIN_DIR) -name '*.s')

# Generating object names from C & S files
FILES_OBJ := $(CFILES:.c=.c.o) $(SFILES:.s=.s.o)
FILES_LYN := $(FILES_OBJ:.o=.lyn.event)

# ---------------------------------
# MAIN TARGETS (OUTPUT ROM & CLEAN)

$(OUTPUT_ROM): $(MAIN) $(BASE_ROM) $(FILES_LYN)
	@echo "$(notdir $<) -> $(notdir $@)"
	@cp -f $(BASE_ROM) $(OUTPUT_ROM)
	@$(EA) A FE8 "-output:$(OUTPUT_ROM)" "-input:$(MAIN)"

clean:
	@echo "cleaning..."
	@rm -f $(FILES_OBJ) $(FILES_LYN) $(OUTPUT_ROM)
	@echo "done."

# -----------------
# FILE TYPE TARGETS

%.lyn.event: %.o $(LYN_LIBS)
	@echo "$(notdir $<) -> $(notdir $@)"
	@$(LYN) $< $(LYNFLAGS) > $@

%.c.o: %.c
	@echo "$(notdir $<) -> $(notdir $@)"
	@$(CC) $(CCFLAGS) -c $< -o $@ $(ERROR_FILTER)

%.s.o: %.s
	@echo "$(notdir $<) -> $(notdir $@)"
	@$(AS) $(ASFLAGS) $< -o $@ $(ERROR_FILTER)
