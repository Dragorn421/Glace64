ROMNAME := glace64

ifeq ($(N64_GCCPREFIX),)
  ifeq ($(N64_INST),)
    $(error Neither N64_GCCPREFIX nor N64_INST is set.)
  else
    N64_GCCPREFIX := $(N64_INST)
  endif
endif
N64_INST := ./libdragon_install
$(info Using N64_INST = $(N64_INST))

all: $(ROMNAME).z64
.PHONY: all

BUILD_DIR = build
SOURCE_DIR = src
include $(N64_INST)/include/n64.mk

C_FILES = $(shell cd src && find . -name '*.c')
OBJS = $(addprefix $(BUILD_DIR)/,$(C_FILES:.c=.o))

$(ROMNAME).z64: N64_ROM_TITLE = "Glace64"

$(BUILD_DIR)/$(ROMNAME).elf: $(OBJS)

clean:
	rm -rf $(BUILD_DIR) *.z64
.PHONY: clean

-include $(wildcard $(BUILD_DIR)/*.d)
