MAKE=make
SUBDIRS=tutorial3/udpserver

clang: $(SUBDIRS)
	$(MAKE) CC=clang -C $<

gcc: $(SUBDIRS)
	$(MAKE) CC=gcc -C $< 

.PHONY: clean
clean: $(SUBDIRS)
	$(MAKE) clean -C $<