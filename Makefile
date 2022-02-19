MAKE=make
SUBDIRS=tutorial2/udpserver

clang: $(SUBDIRS)
	$(MAKE) CC=clang -C $<

gcc: $(SUBDIRS)
	$(MAKE) CC=gcc -C $< 

.PHONY: clean
clean: $(SUBDIRS)
	$(MAKE) clean -C $<