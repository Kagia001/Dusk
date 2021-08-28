##
# Light- and dark mode switcher
#
# @file
# @version 0.1

all: dusk

dusk:
	gcc -I /usr/include /usr/include/opencv4 -L /usr/lib -O -Wall dusk.c -o dusk


clean:
	rm dusk
