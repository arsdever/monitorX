ALL_SOURCES := $(shell find . -name '*.cpp' -o -name '*.h' -o -name Makefile)

.PHONY : build rebuild

build : $(ALL_SOURCES)
	$(MAKE) -C collector
	$(MAKE) -C grapher
	$(MAKE) -C info_getter_api
	$(MAKE) -C application
	$(MAKE) -C extras