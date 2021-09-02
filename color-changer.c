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



char fg[8];
char bg[8];
char cursor[8];
char contrast[8];
char black1[8];
char black2[8];
char red1[8];
char red2[8];
char green1[8];
char green2[8];
char yellow1[8];
char yellow2[8];
char blue1[8];
char blue2[8];
char magenta1[8];
char magenta2[8];
char cyan1[8];
char cyan2[8];
char white1[8];
char white2[8];
char emacstheme[64];
char dwmtextbg[8];
char dwmtextfg[8];
char dwmborder[8];
char dwmbordersel[8];
char dwmtagbg[8];
char dwmtagfg[8];
char dwmtagselbg[8];
char dwmtagselfg[8];
char dwmtitlebg[8];
char dwmtitlefg[8];
char dwmtitleselbg[8];
char dwmtitleselfg[8];
char gtktheme[64];

void applypywal(){
  FILE *pywalfile;
  char buffer[64];

  /* make JSON for pywal using cJSON */
  cJSON *pywalcjson = cJSON_CreateObject();

  cJSON_AddStringToObject(pywalcjson, "wallpaper", "None");
  cJSON_AddStringToObject(pywalcjson, "alpha", "100");

  cJSON *special = cJSON_AddObjectToObject(pywalcjson, "special");
  cJSON_AddStringToObject(special, "background", bg);
  cJSON_AddStringToObject(special, "foreground", fg);
  cJSON_AddStringToObject(special, "cursor", cursor);

  cJSON *colors = cJSON_AddObjectToObject(pywalcjson, "colors");
  cJSON_AddStringToObject(colors, "color0",  black1);
  cJSON_AddStringToObject(colors, "color1",  red1);
  cJSON_AddStringToObject(colors, "color2",  green1);
  cJSON_AddStringToObject(colors, "color3",  yellow1);
  cJSON_AddStringToObject(colors, "color4",  blue1);
  cJSON_AddStringToObject(colors, "color5",  magenta1);
  cJSON_AddStringToObject(colors, "color6",  cyan1);
  cJSON_AddStringToObject(colors, "color7",  white1);
  cJSON_AddStringToObject(colors, "color8",  black2);
  cJSON_AddStringToObject(colors, "color9",  red2);
  cJSON_AddStringToObject(colors, "color10", green2);
  cJSON_AddStringToObject(colors, "color11", yellow2);
  cJSON_AddStringToObject(colors, "color12", blue2);
  cJSON_AddStringToObject(colors, "color13", magenta2);
  cJSON_AddStringToObject(colors, "color14", cyan2);
  cJSON_AddStringToObject(colors, "color15", white2);

  /* Write JSON */
  sprintf(buffer, "%s/.cache/dusk/pywal", getenv("HOME"));

  pywalfile=fopen(buffer, "w");
  fputs(cJSON_Print(pywalcjson), pywalfile);
  fclose(pywalfile);

  /* Load JSON to pywal */
  sprintf(buffer, "wal -q --theme %s/.cache/dusk/pywal", getenv("HOME"));
  system(buffer);

  cJSON_Delete(pywalcjson);
}

void applyemacs(){
  char buffer[128];
  sprintf(buffer, "emacsclient --eval \"  (load-theme '%s)\"", emacstheme);
  system(buffer);
}

void applydwm(){
  FILE *xresources;
  char buffer[512];

  sprintf(buffer, "%s/.cache/dusk/xresources", getenv("HOME"));
  xresources = fopen(buffer, "w");

  /* Make xresources for DWM */
  sprintf(buffer,
          "dwm.normbgcolor:        %s\n"
          "dwm.normfgcolor:        %s\n"
          "dwm.normbordercolor:    %s\n"
          "dwm.selbordercolor:     %s\n"
          "dwm.tagsnormbgcolor:    %s\n"
          "dwm.tagsnormfgcolor:    %s\n"
          "dwm.tagsselbgcolor:     %s\n"
          "dwm.tagsselfgcolor:     %s\n"
          "dwm.titlenormbgcolor:   %s\n"
          "dwm.titlenormfgcolor:   %s\n"
          "dwm.titleselbgcolor:    %s\n"
          "dwm.titleselfgcolor:    %s\n",
          dwmtextbg,
          dwmtextfg,
          dwmborder,
          dwmbordersel,
          dwmtagbg,
          dwmtagfg,
          dwmtagselbg,
          dwmtagselfg,
          dwmtitlebg,
          dwmtitlefg,
          dwmtitleselbg,
          dwmtitleselfg);

  fprintf(xresources, buffer);
  fclose(xresources);

  /* Apply xresources with the xrdb patch and hte dwmc patch */
  system("xrdb -merge ~/.cache/dusk/xresources");
  system("~/src/dwm/patch/dwmc xrdb");

  /* Change x root window color */
  sprintf(buffer, "xsetroot -solid \"%s\"", bg);
  system(buffer);
}

void applygtk(){
  char buffer[128];
  FILE *xsettings;

  sprintf(buffer, "%s/.xsettingsd", getenv("HOME"));
  xsettings = fopen(buffer, "w");
  fprintf(xsettings, "Net/ThemeName \"%s\"", gtktheme);
  fclose(xsettings);
  system("killall -HUP xsettingsd");
}

void changescheme(int color){
  char buffer[256];

  /* make sure ~/.cache/dusk exists */
  sprintf(buffer, "%s/.cache/dusk", getenv("HOME"));
  mkdir(buffer, 0755);

  /* Select colors from the macros in config.h depending on the color scheme */
  switch (color) {
  case 1: {
    /* light mode */
    strcpy(fg, LFG);
    strcpy(bg, LBG);
    strcpy(cursor, LCURSOR);
    strcpy(contrast, LCONTRAST);
    strcpy(black1, LBLACK1);
    strcpy(black2, LBLACK2);
    strcpy(red1, LRED1);
    strcpy(red2, LRED2);
    strcpy(green1, LGREEN1);
    strcpy(green2, LGREEN2);
    strcpy(yellow1, LYELLOW1);
    strcpy(yellow2, LYELLOW2);
    strcpy(blue1, LBLUE1);
    strcpy(blue2, LBLUE2);
    strcpy(magenta1, LMAGENTA1);
    strcpy(magenta2, LMAGENTA2);
    strcpy(cyan1, LCYAN1);
    strcpy(cyan2, LCYAN2);
    strcpy(white1, LWHITE1);
    strcpy(white2, LWHITE2);
    strcpy(emacstheme, LEMACS);
    strcpy(dwmtextbg, LDWMTEXTBG);
    strcpy(dwmtextfg, LDWMTEXTFG);
    strcpy(dwmborder, LDWMBORDER);
    strcpy(dwmbordersel, LDWMBORDERSEL);
    strcpy(dwmtagbg, LDWMTAGBG);
    strcpy(dwmtagfg, LDWMTAGFG);
    strcpy(dwmtagselbg, LDWMTAGSELBG);
    strcpy(dwmtagselfg, LDWMTAGSELFG);
    strcpy(dwmtitlebg, LDWMTITLEBG);
    strcpy(dwmtitlefg, LDWMTITLEFG);
    strcpy(dwmtitleselbg, LDWMTITLESELBG);
    strcpy(dwmtitleselfg, LDWMTITLESELFG);
    strcpy(gtktheme, LGTKTHEME);
    printf("%s\n", "light mode");
    break;
  }
  case 0: {
    /* dark mode */
    strcpy(fg, DFG);
    strcpy(bg, DBG);
    strcpy(cursor, DCURSOR);
    strcpy(contrast, DCONTRAST);
    strcpy(black1, DBLACK1);
    strcpy(black2, DBLACK2);
    strcpy(red1, DRED1);
    strcpy(red2, DRED2);
    strcpy(green1, DGREEN1);
    strcpy(green2, DGREEN2);
    strcpy(yellow1, DYELLOW1);
    strcpy(yellow2, DYELLOW2);
    strcpy(blue1, DBLUE1);
    strcpy(blue2, DBLUE2);
    strcpy(magenta1, DMAGENTA1);
    strcpy(magenta2, DMAGENTA2);
    strcpy(cyan1, DCYAN1);
    strcpy(cyan2, DCYAN2);
    strcpy(white1, DWHITE1);
    strcpy(white2, DWHITE2);
    strcpy(emacstheme, DEMACS);
    strcpy(dwmtextbg, DDWMTEXTBG);
    strcpy(dwmtextfg, DDWMTEXTFG);
    strcpy(dwmborder, DDWMBORDER);
    strcpy(dwmbordersel, DDWMBORDERSEL);
    strcpy(dwmtagbg, DDWMTAGBG);
    strcpy(dwmtagfg, DDWMTAGFG);
    strcpy(dwmtagselbg, DDWMTAGSELBG);
    strcpy(dwmtagselfg, DDWMTAGSELFG);
    strcpy(dwmtitlebg, DDWMTITLEBG);
    strcpy(dwmtitlefg, DDWMTITLEFG);
    strcpy(dwmtitleselbg, DDWMTITLESELBG);
    strcpy(dwmtitleselfg, DDWMTITLESELFG);
    strcpy(gtktheme, DGTKTHEME);
    printf("%s\n", "dark mode");
    break;
  }
  default:
    fprintf(stderr, "color may only be 0 or 1");
    break;
  }

  /* Apply colors when macros are set in config.h */
  if (PYWAL) {
    applypywal();
  }

  if (EMACS) {
    applyemacs();
  }

  if (DWM) {
    applydwm();
  }

  if (GTK) {
    applygtk();
  }
}
