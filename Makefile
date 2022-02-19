MAKE=make
SUBDIRS=tutorial2/udpserver

all: $(SUBDIRS)
	$(MAKE) -C $<

.PHONY: clean
clean: $(SUBDIRS)
	$(MAKE) clean -C $<