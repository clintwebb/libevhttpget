## make file for libevhttpget

MAINFILE=libevhttpget.so
LIBVER=1.0.1
LIBFILE=$(MAINFILE).$(LIBVER)
SONAME=$(MAINFILE).1

all: $(LIBFILE)


LIBS=-levent -lparseurl -lexpbuf
ARGS=
REQH=/usr/include/parseurl.h /usr/include/expbuf.h

# Need to be able to make 'man-pages' as well.  Not sure where to get the source for those... 

libevhttpget.o: evhttpget.c evhttpget.h $(REQH)
	gcc -c -fPIC evhttpget.c -o $@ $(ARGS)

$(LIBFILE): libevhttpget.o
	gcc -shared -Wl,-soname,$(SONAME) -o $@ $^


lib-test: lib-test.c $(REQH) /usr/include/evhttpget.h
	gcc lib-test.c -o $@ -levhttpget -lexpbuf -lparseurl -levent

install: $(LIBFILE) evhttpget.h
	cp evhttpget.h /usr/include/
	cp $(LIBFILE) /usr/lib/
	@-test -e /usr/lib/$(MAINFILE) && rm /usr/lib/$(MAINFILE)
	ln -s /usr/lib/$(LIBFILE) /usr/lib/$(MAINFILE)
	nice ldconfig
	@mkdir tmp.install
	@cp doc/* tmp.install/
	@gzip tmp.install/*.3
	@cp -v tmp.install/*.3.gz $(MANPATH)/man3/
	@rm -r tmp.install
	@echo "Install complete."

clean:
	@-[ -e libevhttpget.o ] && rm libevhttpget.o
	@-[ -e libevhttpget.so* ] && rm libevhttpget.so*
	@-test -e lib-test && rm lib-test

