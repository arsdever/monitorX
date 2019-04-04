ALL_SOURCES := $(shell find . -name '*.cpp' -o -name '*.h' -o -name Makefile)

.PHONY : build rebuild

build : $(ALL_SOURCES)
	$(MAKE) -C collector
	$(MAKE) -C grapher
	$(MAKE) -C comport
	$(MAKE) -C info_getter_api
	$(MAKE) -C provider
	$(MAKE) -C application