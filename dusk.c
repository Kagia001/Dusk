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
#include <cjson/cJSON.h>
#include <stdio.h>
#include <sys/stat.h>
#include <systemd/sd-bus.h>

#include "config.h"
#include "light-detector.c"
#include "color-changer.c"

int main(int argc, char *argv[]) {
  int light;
  int color=1;

  if (argc == 1) {              /* Automatically change themes when no argument is given */

    changescheme(1);            /* Start with light theme as default */

    for(;;){
      light=lightvalue();       /* Get ambient light */
      printf("light value: %d\n", light);

      if (light < 0 || light > 100) {
        fprintf(stderr, "light-value must return an integer from 0 to 100.");
      }

      /* If light mode is on and light fall below the treshold, or if dark mode is on and the light goes
        above the treshold, we change modes. This is done instead of having a single treshold to avoid
       frequent changing when the light level is close to the treshold */
      if ((light > LIGHT_HIGH_TRESH && color == 0) || (light < LIGHT_LOW_TRESH && color == 1)) {
        color = !color;

        switch (color) {
        case 1: {
          changescheme(1);
          break;
        }
        case 0: {
          changescheme(0);
          break;
        }
        default:
          break;
        }
      }

      sleep(REFRESH_TIME);
    }


  } else {
    /* We got an argument, so either change to light or dark mode. */
    switch (atoi(argv[1])) {
    case 1: {
      changescheme(1);
      break;
    }
    case 0: {
      changescheme(0);
      break;
    }
    default:
      break;
    }
  }
  return 0;
}
