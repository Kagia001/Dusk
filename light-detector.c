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

/* This file is meant for you change! The default uses your webcam to get an
   ambient light value, but you can change it to use whatever you want. For
   example you could connect an Arduino with a light sensor, connect your
   smart light switch, use the time, or whatever you want! Just make a function
   called lightvalue which returns an int from 0 to 100. */


int lightvalue(){
  double brightness;

  sd_bus *userbus=NULL;
  sd_bus_message *sdbusmessage = NULL;

  /* start clight if it isn't running */
  if(system("pidof -x clight > /dev/null") != 0){
    printf("%s\n", "starting clight");
    system("clight --no-gamma --no-dimmer --no-dpms --no-kbd --no-auto-calib &");
    sleep(5);
  }

  /* get user dbus */
  sd_bus_default_user(&userbus);

  /* refresh clight */
  sd_bus_call_method(userbus,
                     "org.clight.clight",
                     "/org/clight/clight",
                     "org.clight.clight",
                     "Capture",
                     NULL,
                     NULL,
                     "bb",
                     "0",
                     "1");


  /* get clight ambient light value */
  sd_bus_get_property(userbus,
                      "org.clight.clight",
                      "/org/clight/clight",
                      "org.clight.clight",
                      "AmbientBr",
                      NULL,
                      &sdbusmessage,
                      "d");

  sd_bus_message_read(sdbusmessage, "d", &brightness);

  sd_bus_message_unref(sdbusmessage);
  sd_bus_unref(userbus);

  return brightness * 100;       /* return brightness as int from 0-100 */
}
