##
# Light- and dark mode switcher
#
# @file
# @version 0.1

all: light-dark-mode-changer paleta

light-dark-mode-changer:
	gcc -I /usr/include -L /usr/lib -O -Wall light-dark-mode-changer.c -o light-dark-mode-changer -lusb
	gcc -I /usr/include -L /usr/lib -O -Wall paleta/paleta.c -o paleta/paleta


install: all
	ln -s /home/karl/src/light-dark-mode/light-dark-mode-changer /home/karl/bin/light-dark-mode-changer
clean:
	rm /home/karl/bin/light-dark-mode-changer
	rm light-dark-mode-changer
