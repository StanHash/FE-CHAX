.SUFFIXES:
.PHONY: hack all clean

include Tools.mak

# Setting C/ASM include directories up
INCLUDE_DIRS := Tools/libgbafe
INCFLAGS     := $(foreach dir, $(INCLUDE_DIRS), -I "$(dir)")

# setting up compilation flags
ARCH    := -mcpu=arm7tdmi -mthumb -mthumb-interwork
CFLAGS  := $(ARCH) $(INCFLAGS) -Wall -Os -mtune=arm7tdmi -fomit-frame-pointer -ffast-math -ffreestanding
ASFLAGS := $(ARCH) $(INCFLAGS)

# setting up cache dir
CACHE_DIR := .MkCache
$(shell mkdir -p $(CACHE_DIR) > /dev/null)

# defining dependency directory
DEPSDIR := $(CACHE_DIR)

# lyn options
LYNLIB := Tools/libgbafe/fe8u.o

# Finding all possible source files
CFILES   := $(shell find -type f -name '*.c')
SFILES   := $(shell find -type f -name '*.s')
OFILES   := $(CFILES:.c=.o) $(SFILES:.s=.o)
ASMFILES := $(CFILES:.c=.asm)
LYNFILES := $(OFILES:.o=.lyn.event)
DMPFILES := $(OFILES:.o=.dmp)
DEPFILES := $(addprefix $(DEPSDIR)/, $(notdir $(CFILES:.c=.d)))

# EA Files
EVENT_MAIN     := Main.event
EVENT_MAIN_DEP := $(CACHE_DIR)/Main.d

EVENT_SYMBOLS  := HACK.sym.event

# ROMs
ROM_SOURCE     := FE8U.gba
ROM_TARGET     := HACK.gba

# defining C dependency flags
CDEPFLAGS = -MMD -MT "$*.o" -MT "$*.asm" -MF "$(DEPSDIR)/$(notdir $*).d" -MP

# All files
ALL_FILES := $(EVENT_MAIN_DEP) $(ROM_TARGET) $(EVENT_SYMBOLS) $(OFILES) $(ASMFILES) $(LYNFILES) $(DMPFILES)

# Variable listing all text files in the writans directory
# The text installer depends on them (in case there was any change)
# (Too lazy to code a dependency thingy for that too)
WRITANS_ALL_TEXT := $(wildcard Writans/*.txt)

# ------------------
# PHONY TARGET RULES

hack: $(ROM_TARGET);
all:  $(ALL_FILES);

clean:
	@echo "cleaning..."
	@rm -f $(ALL_FILES)
	@echo "done."

# -------------------
# ACTUAL TARGET RULES

$(ROM_TARGET): $(EVENT_MAIN) $(EVENT_MAIN_DEP) $(ROM_SOURCE)
	$(PREPROCESS_MESSAGE)
	@cp -f "$(ROM_SOURCE)" "$(ROM_TARGET)"
	@$(EA) A FE8 -output $(ROM_TARGET) -input $(EVENT_MAIN) -symOutput $(EVENT_SYMBOLS)

$(EVENT_MAIN_DEP): $(EVENT_MAIN)
	$(PREPROCESS_MESSAGE)
	@$(EA) A FE8 -output $(ROM_TARGET) -input $(EVENT_MAIN) -quiet -MM -MG -MT $(EVENT_MAIN_DEP) -MF $(EVENT_MAIN_DEP)
	@sed -i s/\\\\/\\//g $(EVENT_MAIN_DEP)

# -------------------
# SPECIAL FILES RULES

Spritans/Portraits.event: Spritans/PortraitList
	$(GENMUGS) $< $@

Writans/Text.event Writans/TextDefinitions.event: $(WRITANS_ALL_TEXT)
	$(TEXTPROCESS) Writans/TextMain.txt Writans/Text.event Writans/TextDefinitions.event

# -------------
# PATTERN RULES

# C to ASM rule
%.asm: %.c
	$(PREPROCESS_MESSAGE)
	@$(CC) $(CFLAGS) $(CDEPFLAGS) -S $< -o $@ -fverbose-asm $(ERROR_FILTER)

%.asm: %.cpp
	$(PREPROCESS_MESSAGE)
	@$(CC) $(CFLAGS) $(CDEPFLAGS) -S $< -o $@ -fverbose-asm -fno-rtti $(ERROR_FILTER)

# C to OBJ rule
%.o: %.c
	$(PREPROCESS_MESSAGE)
	@$(CC) $(CFLAGS) $(CDEPFLAGS) -c $< -o $@ $(ERROR_FILTER)

# ASM to OBJ rule
%.o: %.s
	@echo "$(notdir $<) => $(notdir $@)"
	@$(AS) $(ARCH) -I $(dir $<) $< -o $@ $(ERROR_FILTER)

# OBJ to DMP rule
%.dmp: %.o
	$(PREPROCESS_MESSAGE)
	@$(OBJCOPY) -S $< -O binary $@

# OBJ to EVENT rule
%.lyn.event: %.o $(LYNLIB)
	$(PREPROCESS_MESSAGE)
	@$(LYN) $< $(LYNLIB) > $@

# PNG to 4bpp rule
%.4bpp: %.png
	$(PREPROCESS_MESSAGE)
	@$(GBAGFX) $< $@

# PNG to gbapal rule
%.gbapal: %.png
	$(PREPROCESS_MESSAGE)
	@$(GBAGFX) $< $@

# Anything to lz rule
%.lz: %
	$(PREPROCESS_MESSAGE)
	@$(GBAGFX) $< $@

# Formatted text to insertable binary
# Nulling output because it's annoying
%.fetxt.bin: %.fetxt
	$(PARSEFILE) $< -o $@ > /dev/null

# PNG to insertable portrait rule
%_mug.dmp %_palette.dmp %_frames.dmp %_minimug.dmp: %.png
	$(PORTRAITFORMATTER) $<

# CSV+NMM to event
# Untested
%.event: %.csv %.nmm
	$(C2EA) $*.csv $*.event $(ROM_SOURCE)

# TMX to event + dmp
%.event %_data.dmp: %.tmx
	$(TMX2EA) $*.tmx $*.event

# --------------------
# INCLUDE DEPENDENCIES

-include $(DEPFILES)
-include $(EVENT_MAIN_DEP)
