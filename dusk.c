/**
 * Project: AVR ATtiny USB Tutorial at http://codeandlife.com/
 * Author: Joonas Pihlajamaa, joonas.pihlajamaa@iki.fi
 * Based on V-USB example code by Christian Starkjohann
 * Copyright: (C) 2012 by Joonas Pihlajamaa
 * License: GNU GPL v3 (see License.txt)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// this is libusb, see http://libusb.sourceforge.net/ 
#include <unistd.h>
#include <usb.h>

#include "config.h"

// same as in main.c
#define USB_LED_OFF 0
#define USB_LED_ON  1
#define USB_DATA_OUT 2
#define USB_DATA_WRITE 3
#define USB_DATA_IN 4
#define LIGHT_LOW_TRESH 500
#define LIGHT_HIGH_TRESH 550

#define LIGHTXRESOURCES "dwm.normbordercolor:    #ebdbb2\n"\
                        "dwm.normbgcolor:        #fbf1c7\n"\
                        "dwm.normfgcolor:        #3c3836\n"\
                        "dwm.selbordercolor:     #d65d0e\n"\
                        "dwm.tagsnormbgcolor:    #d65d0e\n"\
                        "dwm.tagsnormfgcolor:    #fbf1c7\n"\
                        "dwm.tagsselbgcolor:     #458588\n"\
                        "dwm.tagsselfgcolor:     #fbf1c7\n"\
                        "dwm.titlenormbgcolor:   #fbf1c7\n"\
                        "dwm.titlenormfgcolor:   #d65d0e\n"\
                        "dwm.titleselbgcolor:    #d65d0e\n"\
                        "dwm.titleselfgcolor:    #fbf1c7\n"


#define DARKXRESOURCES  "dwm.normbordercolor:    #3c3836\n"\
                        "dwm.normbgcolor:        #282828\n"\
                        "dwm.normfgcolor:        #ebdbb2\n"\
                        "dwm.selbordercolor:     #d65d0e\n"\
                        "dwm.tagsnormbgcolor:    #d65d0e\n"\
                        "dwm.tagsnormfgcolor:    #282828\n"\
                        "dwm.tagsselbgcolor:     #458588\n"\
                        "dwm.tagsselfgcolor:     #ebdbb2\n"\
                        "dwm.titlenormbgcolor:   #282828\n"\
                        "dwm.titlenormfgcolor:   #d65d0e\n"\
                        "dwm.titleselbgcolor:    #d65d0e\n"\
                        "dwm.titleselfgcolor:    #282828\n"


// used to get descriptor strings for device identification
static int usbGetDescriptorString(usb_dev_handle *dev, int index, int langid, 
                                  char *buf, int buflen) {
    char buffer[256];
    int rval, i;

	// make standard request GET_DESCRIPTOR, type string and given index 
    // (e.g. dev->iProduct)
	rval = usb_control_msg(dev, 
        USB_TYPE_STANDARD | USB_RECIP_DEVICE | USB_ENDPOINT_IN, 
        USB_REQ_GET_DESCRIPTOR, (USB_DT_STRING << 8) + index, langid, 
        buffer, sizeof(buffer), 1000);
        
    if(rval < 0) // error
		return rval;
	
    // rval should be bytes read, but buffer[0] contains the actual response size
	if((unsigned char)buffer[0] < rval)
		rval = (unsigned char)buffer[0]; // string is shorter than bytes read
	
	if(buffer[1] != USB_DT_STRING) // second byte is the data type
		return 0; // invalid return type
		
	// we're dealing with UTF-16LE here so actual chars is half of rval,
	// and index 0 doesn't count
	rval /= 2;
	
	// lossy conversion to ISO Latin1 
	for(i = 1; i < rval && i < buflen; i++) {
		if(buffer[2 * i + 1] == 0)
			buf[i-1] = buffer[2 * i];
		else
			buf[i-1] = '?'; // outside of ISO Latin1 range
	}
	buf[i-1] = 0;
	
	return i-1;
}

static usb_dev_handle * usbOpenDevice(int vendor, char *vendorName, 
                                      int product, char *productName) {
	struct usb_bus *bus;
	struct usb_device *dev;
	char devVendor[256], devProduct[256];
    
	usb_dev_handle * handle = NULL;
	
	usb_init();
	usb_find_busses();
	usb_find_devices();
	
	for(bus=usb_get_busses(); bus; bus=bus->next) {
		for(dev=bus->devices; dev; dev=dev->next) {			
			if(dev->descriptor.idVendor != vendor ||
               dev->descriptor.idProduct != product)
                continue;
                
            // we need to open the device in order to query strings 
            if(!(handle = usb_open(dev))) {
                fprintf(stderr, "Warning: cannot open USB device: %s\n",
                    usb_strerror());
                continue;
            }
            
            // get vendor name 
            if(usbGetDescriptorString(handle, dev->descriptor.iManufacturer, 0x0409, devVendor, sizeof(devVendor)) < 0) {
                fprintf(stderr, 
                    "Warning: cannot query manufacturer for device: %s\n", 
                    usb_strerror());
                usb_close(handle);
                continue;
            }
            
            // get product name 
            if(usbGetDescriptorString(handle, dev->descriptor.iProduct, 
               0x0409, devProduct, sizeof(devVendor)) < 0) {
                fprintf(stderr, 
                    "Warning: cannot query product for device: %s\n", 
                    usb_strerror());
                usb_close(handle);
                continue;
            }
            
            if(strcmp(devVendor, vendorName) == 0 && 
               strcmp(devProduct, productName) == 0)
                return handle;
            else
                usb_close(handle);
		}
	}
	
	return NULL;
}

int light_value() {
	usb_dev_handle *handle = NULL;
    int nBytes = 0;
    char buffer[256];


	handle = usbOpenDevice(0x16C0, USBDRVNAME, 0x05DC, "ArdUSB.LDR");
	
	if(handle == NULL) {
		fprintf(stderr, "Could not find USB device!\n");
		exit(1);
	}

	nBytes = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN, USB_DATA_OUT, 0, 0, (char *)buffer, sizeof(buffer), 5000);

	if(nBytes < 0)
		fprintf(stderr, "USB error: %s\n", usb_strerror());
		
	usb_close(handle);

    return atoi(buffer);
}

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
  FILE *xresources;
  /* apply dark mode where possible */
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
        light=light_value();    /* Get ambient light */
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
