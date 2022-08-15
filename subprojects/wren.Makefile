SOURCE_ARCHIVE := https://github.com/wren-lang/wren/archive/refs/tags/0.4.0.zip
CWD := $(shell pwd)

all: simulator device

simulator: wren/projects/make/Makefile
# Make static library archive for host machine
	@make -C wren/projects/make wren config=debug_64bit

# TODO: Disable the Wren internal parser in production (with JS source snapshots)
device: wren/projects/make/Makefile
# Cross-Compile Wren for Playdate devices as a static library archive
	@make -C wren/projects/make wren config=release_arm CC=arm-none-eabi-gcc

#################################################
# Download Wren Sources
#################################################

wren/projects/make/Makefile:
	@echo "Downloading Wren sources…"
	@curl -L $(SOURCE_ARCHIVE) --output /tmp/wren.zip
	@echo "Unzipping Wren sources…"
	@unzip -q /tmp/wren.zip -d wren
	@mv -f wren/wren-0.4.0/* wren/wren-0.4.0/.gitignore wren
	@rm -r wren/wren-0.4.0
