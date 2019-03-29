SUBDIRS := $(shell ls -d)

.PHONY : build rebuild

build : $(SUBDIRS)
	$(MAKE) -C src

rebuild :
	rm -fr build
	make build