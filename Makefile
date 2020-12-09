CC      := gcc
CFLAGS  := -O2 -Wall $(shell pkg-config --cflags vte-2.91)
LIBS    := $(shell pkg-config --libs vte-2.91)

PREFIX  ?= /usr/local
BINDIR  ?= ${PREFIX}/bin
DATADIR ?= ${PREFIX}/share
MANDIR  ?= ${DATADIR}/man

mterm: main.c config.h; $(CC) $(CFLAGS) $? $(LIBS) -o $@

run: mterm; ./mterm

install: mterm mterm.desktop mterm.1
	install -Dm755 mterm ${DESTDIR}${BINDIR}/mterm
	install -Dm644 mterm.desktop ${DESTDIR}${DATADIR}/applications/mterm.desktop
	install -Dm644 mterm.1 ${DESTDIR}${MANDIR}/man1/mterm.1

uninstall:
	rm -f ${DESTDIR}${BINDIR}/mterm
	rm -f ${DESTDIR}${DATADIR}/applications/mterm.desktop
	rm -f ${DESTDIR}${MANDIR}/man1/mterm.1

clean:
	rm mterm
