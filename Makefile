PLUGINS = WSTD_EQ
TARGETS = $(PLUGINS:%=%/generated.cpp)

all: build

build: pregen
	$(foreach p,$(PLUGINS),$(MAKE) -C $(p))

pregen: $(TARGETS)

%/generated.cpp: %.pd
	# generate files
	hvcc %.pd -n % -m %.json -o % -g dpf -p dep/heavylib/ dep/ --copyright "Copyright (c) Wasted Audio 2023 - GPL-3.0-or-later"
	cp override/* %/plugin/source/