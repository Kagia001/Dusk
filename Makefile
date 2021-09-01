##
# Light- and dark mode switcher
#
# @file
# @version 0.1

all: dusk

dusk:
	gcc -I /usr/include -L /usr/lib -O -Wall `pkg-config --cflags --libs libsystemd libcjson` dusk.c -o dusk

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f dusk ${DESTDIR}${PREFIX}/bin

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/dusk

clean:
	rm dusk
