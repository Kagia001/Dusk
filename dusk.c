/*
  Copyright 2021 Karl Ragnar Giese, karl@giese.no


  This file is part of Dusk.

  Dusk is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  Dusk is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Dusk; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "config.h"
#include "light-detector.c"

#define LIGHT_LOW_TRESH 500
#define LIGHT_HIGH_TRESH 550

#define LIGHTXRESOURCES "dwm.normbordercolor:    #ebdbb2\n" \
  "dwm.normbgcolor:        #fbf1c7\n"                       \
  "dwm.normfgcolor:        #3c3836\n"                       \
  "dwm.selbordercolor:     #d65d0e\n"                       \
  "dwm.tagsnormbgcolor:    #d65d0e\n"                       \
  "dwm.tagsnormfgcolor:    #fbf1c7\n"                       \
  "dwm.tagsselbgcolor:     #458588\n"                       \
  "dwm.tagsselfgcolor:     #fbf1c7\n"                       \
  "dwm.titlenormbgcolor:   #fbf1c7\n"                       \
  "dwm.titlenormfgcolor:   #d65d0e\n"                       \
  "dwm.titleselbgcolor:    #d65d0e\n"                       \
  "dwm.titleselfgcolor:    #fbf1c7\n"


#define DARKXRESOURCES  "dwm.normbordercolor:    #3c3836\n" \
  "dwm.normbgcolor:        #282828\n"                       \
  "dwm.normfgcolor:        #ebdbb2\n"                       \
  "dwm.selbordercolor:     #d65d0e\n"                       \
  "dwm.tagsnormbgcolor:    #d65d0e\n"                       \
  "dwm.tagsnormfgcolor:    #282828\n"                       \
  "dwm.tagsselbgcolor:     #458588\n"                       \
  "dwm.tagsselfgcolor:     #ebdbb2\n"                       \
  "dwm.titlenormbgcolor:   #282828\n"                       \
  "dwm.titlenormfgcolor:   #d65d0e\n"                       \
  "dwm.titleselbgcolor:    #d65d0e\n"                       \
  "dwm.titleselfgcolor:    #282828\n"


void lightmode(){
  FILE *xresources;
  /* apply light mode where possible */
  system("emacsclient --eval \"(load-theme 'gruvbox-light-medium)\" "); /* EMACS */

  /* DWM via xrdb and dwmc patches*/
  xresources = fopen("/home/karl/.Xresources", "w");
  fprintf(xresources, LIGHTXRESOURCES);
  fclose(xresources);
  system("xrdb ~/.Xresources");
  system(". ~/src/dwm/patch/dwmc xrdb");

  /* Background */
  system("xsetroot -solid \"#fbf1c7\"");

  /* Terminal via paleta, write paleta to cache which is read by fishrc */
  system("~/src/light-dark-mode/paleta/paleta < ~/src/light-dark-mode/gruvbox-light-paleta > ~/.cache/paleta/theme");

  /* GTK via xfsettingsd */
  system("xfconf-query -c xsettings -p /Net/ThemeName -s \"Gruvbox-light\"");
}

void darkmode(){
  FILE *xresources;/* apply dark mode where possible */
  system("emacsclient --eval \"(load-theme 'gruvbox-dark-medium)\" "); /* EMACS */

  /* DWM via xrdb and dwmc patches*/
  xresources = fopen("/home/karl/.Xresources", "w");
  fprintf(xresources, DARKXRESOURCES);
  fclose(xresources);
  system("xrdb ~/.Xresources");
  system(". ~/src/dwm/patch/dwmc xrdb");

  /* Background */
  system("xsetroot -solid \"#282828\"");

  /* Terminal via paleta, write paleta to cache which is read by fishrc */
  system("~/src/light-dark-mode/paleta/paleta < ~/src/light-dark-mode/gruvbox-dark-paleta > ~/.cache/paleta/theme");

  /* GTK via xfsettingsd */
  system("xfconf-query -c xsettings -p /Net/ThemeName -s \"Gruvbox-dark\"");
}

int main(int argc, char *argv[]) {
  int light;
  int color=1;

  lightmode();                /* Start with light mode */

  for(;;){
    light=lightvalue();    /* Get ambient light */

    if (light < 0 || light > 100) {
      fprintf(stderr, "light-value must return an integer from 0 to 100.");
    }

    if ((light > LIGHT_HIGH_TRESH && color == 0) || (light < LIGHT_LOW_TRESH && color == 1)) { /* Hysteresis */
      color = !color;

      switch (color) {
      case 1: {
        lightmode();
        break;
      }
      case 0: {
        darkmode();
        break;
      }
      default:
        break;
      }
    }

    sleep(1);
  }
  return 0;
}
