VERSION := "0.1.0"
PRODUCT := build/app.pdx
SOURCES := src/lib/string.c src/main.c src/wren.c
RESOURCES := Resources

.DEFAULT_GOAL := debug

# CLANGFLAGS += -L "$(shell pwd)/build" build/libwren.a

# Enable GCov code coverage
# debug: OPT += -fprofile-arcs -ftest-coverage
# https://stackoverflow.com/a/879383/1363247
# https://stackoverflow.com/a/23922211/1363247
# debug: LDFLAGS += -fprofile-arcs --specs=nosys.specs

debug: simulator pdc
release: device pdc

simulator: LIBWREN := subprojects/wren/lib/libwren_d.a
device: LIBWREN := subprojects/wren/lib/libwren.a

#################################################
# Script Sources
#################################################

node_modules: package.json
	npm install

SCRIPT_SOURCES = $(shell find src -type f -name '*.wren')
scripts: $(SCRIPT_SOURCES)
	@cp $(SCRIPT_SOURCES) $(RESOURCES)
	# TODO: gzip bundle Wren sources for Playdate distribution

#################################################
# Subprojects
#################################################

# Wren (https://wren.io)
subprojects/wren/lib/libwren_d.a: subprojects/wren.Makefile
	@make -C subprojects -f wren.Makefile simulator
subprojects/wren/lib/libwren.a: subprojects/wren.Makefile
	@make -C subprojects -f wren.Makefile device
build/libwren.a: $(LIBWREN)
	@mkdir -p build && cp $(LIBWREN) build/libwren.a
SOURCES += build/libwren.a

clean-subprojects:
# Clean Wren targets
	@make -C subprojects/wren/projects/make clean
	-rm -rf subprojects/wren/projects/make/obj
	-rm -fR build/libwren.a

#################################################
# Playdate SDK
#################################################

# Locate the Playdate SDK
SDK = ${PLAYDATE_SDK_PATH}
ifeq ($(SDK),)
SDK = $(shell egrep '^\s*SDKRoot' ~/.Playdate/config | head -n 1 | cut -c9-)
$(info $(SDK))
endif

ifeq ($(SDK),)
$(error SDK path not found; set ENV value PLAYDATE_SDK_PATH)
endif

# Configure
VPATH := $(RESOURCES)
SRC := $(SOURCES)
# User directories here
UINCDIR = subprojects/wren/src/include
# ASM sources
UASRC =
# C defines, e.g. -D_DEBUG=1
UDEFS =
# ASM defines
UADEFS =
# Library search paths
ULIBDIR =
# Dependent libraries
ULIBS =
include $(SDK)/C_API/buildsupport/common.mk

$(RESOURCES):
	mkdir -p $(RESOURCES)

simulator: build/libwren.a $(RESOURCES) $(OBJDIR)/pdex.${DYLIB_EXT}
	@touch $(RESOURCES)/pdex.bin
	@cp $(OBJDIR)/pdex.${DYLIB_EXT} $(RESOURCES)

# TODO: release: LDFLAGS += --specs=nosys.specs
device: build/libwren.a $(RESOURCES) $(OBJDIR)/pdex.bin
	cp $(OBJDIR)/pdex.bin $(RESOURCES)

pdc: scripts simulator
	$(PDC) $(PDCFLAGS) $(RESOURCES) $(PRODUCT)

check:
# TODO: Write unit tests
	@echo "No tests" && false

clean: clean-subprojects
