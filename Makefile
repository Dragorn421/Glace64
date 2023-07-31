ROMNAME := glace64

BUILD_DIR = build
SOURCE_DIR = src

ifeq ($(N64_GCCPREFIX),)
  ifeq ($(N64_INST),)
    $(error Neither N64_GCCPREFIX nor N64_INST is set.)
  else
    N64_GCCPREFIX := $(N64_INST)
  endif
endif
N64_INST := ./libdragon_install
$(info Using N64_INST = $(N64_INST))

include $(N64_INST)/include/n64.mk

all: $(ROMNAME).z64
.PHONY: all

C_FILES = $(shell cd src && find . -name '*.c') 
OBJS = $(addprefix $(BUILD_DIR)/,$(C_FILES:.c=.o))

#maybe pull asset conversion into its own makefile somehow?

FILESYSTEM := build/filesystem

# NOTE: just putting a raw .font64 or .sprite or etc in the filesystem is not enough to
# add it to the filesystem here. we need to put the normal assets in /assets, 
# and the Makefile will convert the assets to .sprite and .font64.
## CONVERT AUDIO ASSETS
AUDIOCONV_FLAGS ?=

assets_xm1 = $(wildcard assets/*.xm)
assets_xm2 = $(wildcard assets/*.XM)
assets_ym1 = $(wildcard assets/*.ym)
assets_ym2 = $(wildcard assets/*.YM)

assets_mp3 = $(wildcard assets/*.mp3)

audio_assets_conv = $(addprefix $(FILESYSTEM)/,$(notdir $(assets_xm1:%.xm=%.xm64))) \
              $(addprefix $(FILESYSTEM)/,$(notdir $(assets_xm2:%.XM=%.xm64))) \
              $(addprefix $(FILESYSTEM)/,$(notdir $(assets_ym1:%.ym=%.ym64))) \
              $(addprefix $(FILESYSTEM)/,$(notdir $(assets_ym2:%.YM=%.ym64))) \
              $(addprefix $(FILESYSTEM)/,$(notdir $(assets_mp3:%.mp3=%.mp3)))

# Run audioconv64 on all XM/YM files under assets/
# We do this file by file, but we could even do it just once for the whole
# directory, because audioconv64 supports directory walking.
$(FILESYSTEM)/%.xm64: assets/%.xm
	@mkdir -p $(dir $@)
	@echo "    [AUDIO] $@"
	@$(N64_AUDIOCONV) $(AUDIOCONV_FLAGS) -o $(FILESYSTEM) "$<"
$(FILESYSTEM)/%.xm64: assets/%.XM
	@mkdir -p $(dir $@)
	@echo "    [AUDIO] $@"
	@$(N64_AUDIOCONV) $(AUDIOCONV_FLAGS) -o $(FILESYSTEM) "$<"

$(FILESYSTEM)/%.ym64: assets/%.ym
	@mkdir -p $(dir $@)
	@echo "    [AUDIO] $@"
	$(N64_AUDIOCONV) $(AUDIOCONV_FLAGS) -o $(FILESYSTEM) "$<"
$(FILESYSTEM)/%.ym64: assets/%.YM
	@mkdir -p $(dir $@)
	@echo "    [AUDIO] $@"
	$(N64_AUDIOCONV) $(AUDIOCONV_FLAGS) -o $(FILESYSTEM) "$<"

$(FILESYSTEM)/%.mp3: assets/%.mp3
	@mkdir -p $(dir $@)
	@echo "    [AUDIO] $@"
	cp $< $@

## CONvERT FONT ASSETS
MKFONT_FLAGS ?=

assets_ttf = $(wildcard assets/*.ttf)
font_assets_conv = $(addprefix $(FILESYSTEM)/,$(notdir $(assets_ttf:%.ttf=%.font64)))

# conversion rules, assets -> filesystem
$(FILESYSTEM)/%.font64: assets/%.ttf
	@mkdir -p $(dir $@)
	@echo "    [FONT] $@"
	@$(N64_MKFONT) $(MKFONT_FLAGS) -o $(FILESYSTEM) "$<"


MKSPRITE_FLAGS ?=

assets_png = $(wildcard assets/*.png)
image_assets_conv = $(addprefix $(FILESYSTEM)/,$(notdir $(assets_png:%.png=%.sprite)))

$(FILESYSTEM)/%.sprite: assets/%.png
	@mkdir -p $(dir $@)
	@echo "    [SPRITE] $@"
	@$(N64_MKSPRITE) $(MKSPRITE_FLAGS) -o $(FILESYSTEM) "$<"


# the helix includes, to help helix compile.
$(OBJS): CFLAGS += -I. -Isrc/deps/helix/pub

$(BUILD_DIR)/$(ROMNAME).dfs: $(image_assets_conv) $(audio_assets_conv) $(font_assets_conv)
$(BUILD_DIR)/$(ROMNAME).elf: $(OBJS)

$(ROMNAME).z64: N64_ROM_TITLE = "Glace64"
$(ROMNAME).z64: $(BUILD_DIR)/$(ROMNAME).dfs

clean:
	rm -rf $(BUILD_DIR) *.z64
.PHONY: clean

-include $(shell find $(BUILD_DIR) -name '*.d')
