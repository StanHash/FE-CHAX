.SUFFIXES:
.PHONY: hack all clean

include tools.mak

# Setting C/ASM include directories up
INCLUDE_DIRS := libgbafe
INCFLAGS     := $(foreach dir, $(INCLUDE_DIRS), -I "$(dir)")

# setting up compilation flags
ARCH    := -mcpu=arm7tdmi -mthumb -mthumb-interwork
CFLAGS  := $(ARCH) $(INCFLAGS) -Wall -Os -mtune=arm7tdmi -fomit-frame-pointer -ffast-math
ASFLAGS := $(ARCH) $(INCFLAGS)

# setting up cache dir
CACHE_DIR := .MkCache
$(shell mkdir -p $(CACHE_DIR) > /dev/null)

# defining dependency directory
DEPSDIR := $(CACHE_DIR)

# lyn options
LYNLIB := libgbafe/fe8u.o

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
EVENT_MAIN_DEP := $(CACHE_DIR)/EventMain.d

# ROMs
ROM_SOURCE     := FE8_U.gba
ROM_TARGET     := FEHACK.gba

# pea options
PEAFLAGS   := -D _FE8_ -I bin/EventAssembler/ -T bin/EventAssembler/Tools/
PEADEPFLAGS = -MM -MT $(ROM_TARGET) -MF $(EVENT_MAIN_DEP) -MP -MG -.pp.makedep.outputIsTarget=true

# defining C dependency flags
CDEPFLAGS = -MMD -MT "$*.o" -MT "$*.asm" -MF "$(DEPSDIR)/$(notdir $*).d" -MP

# All files
ALL_FILES := $(EVENT_MAIN_DEP) $(ROM_TARGET) $(OFILES) $(ASMFILES) $(LYNFILES) $(DMPFILES)

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
	@$(EA) A FE8 "-output:$(abspath $(ROM_TARGET))" "-input:$(abspath $(EVENT_MAIN))"

$(EVENT_MAIN_DEP): $(EVENT_MAIN)
	$(PREPROCESS_MESSAGE)
	@$(PEA) $(PEAFLAGS) $(PEADEPFLAGS) "$(EVENT_MAIN)"

# -------------
# PATTERN RULES

# C to ASM rule
%.asm: %.c
	$(PREPROCESS_MESSAGE)
	@$(CC) $(CFLAGS) $(CDEPFLAGS) -S $< -o $@ -fverbose-asm $(ERROR_FILTER)

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
	@lyn $< $(LYNLIB) > $@

# --------------------
# INCLUDE DEPENDENCIES

-include $(DEPFILES)
-include $(EVENT_MAIN_DEP)
