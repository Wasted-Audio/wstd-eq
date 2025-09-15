#!/usr/bin/make -f

include dep/dpf/Makefile.base.mk

PLUGINS = WSTD_EQ
PREGEN = $(PLUGINS:%=%/plugin/source)

all: build

build: pregen
	$(foreach p, $(PLUGINS), $(MAKE) -C $(p);)
	mkdir bin
	$(foreach p, $(PLUGINS), mv $(p)/bin/* bin/;)

pregen: $(PREGEN)

%/plugin/source: %.json %.pd override/*.*
	hvcc $*.pd -m $*.json -n $* -o $* -g dpf -p dep/heavylib/ dep/ --copyright "Copyright (c) Wasted Audio 2023 - GPL-3.0-or-later"
	cp override/*.* $*/plugin/source/
