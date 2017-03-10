# TARGET #

TARGET := 3DS
LIBRARY := 0

ifeq ($(TARGET),3DS)
    ifeq ($(strip $(DEVKITPRO)),)
        $(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>devkitPro")
    endif

    ifeq ($(strip $(DEVKITARM)),)
        $(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
    endif
endif

# PORTLIBS #

export PORTLIBS_PATH	:=	$(DEVKITPRO)/portlibs
PORTLIBS	:=	$(PORTLIBS_PATH)/armv6k $(PORTLIBS_PATH)/3ds

# COMMON CONFIGURATION #

NAME := 3DS EBook Reader

BUILD_DIR := build
OUTPUT_DIR := output
INCLUDE_DIRS := include
SOURCE_DIRS := source

EXTRA_OUTPUT_FILES :=

LIBRARY_DIRS := $(DEVKITPRO)/libctru $(PORTLIBS)
LIBRARIES := sftd sfil freetype jpeg turbojpeg png png16 sf2d citro3d ctru bz2 m z
BUILD_FLAGS :=
RUN_FLAGS :=

#VERSION_PARTS := $(subst ., ,$(shell git describe --tags --abbrev=0))

#VERSION_MAJOR := $(word 1, $(VERSION_PARTS))
#VERSION_MINOR := $(word 2, $(VERSION_PARTS))
#VERSION_MICRO := $(word 3, $(VERSION_PARTS))

# 3DS CONFIGURATION #

TITLE := $(NAME)
DESCRIPTION := Read Ebooks on your 3DS!
AUTHOR := reworks
PRODUCT_CODE := CTR-P-EBOK
UNIQUE_ID := 0xEB001

SYSTEM_MODE := 64MB
SYSTEM_MODE_EXT := Legacy

ICON_FLAGS :=

ROMFS_DIR := romfs
BANNER_AUDIO := meta/audio.wav
BANNER_IMAGE := meta/banner.png
ICON := meta/icon.png

# INTERNAL #

include buildtools/make_base