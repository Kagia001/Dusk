# Dusk

Dusk is a C program which swiches your programs between light and dark mode depending on (here's the kicker!) whatever you want! Many other programs only change colors depending on time, but Dusk is written to make it simple to use anything to change the colors. By default there is a script which gets ambient light from your webcam, but you can easily make it use something else. Dusk is completely configured in it's source code, similar to Suckless' programs.

## Installation

Configuring the program with source code sadly means that you can't install Dusk from a package manager. However, it isn't especially difficult to install from source.

```bash
git clone https://github.com/Kagia001/Dusk.git
cd Dusk
```
here's where you configure Dusk
``` bash
sudo make install
```

## Usage

``` bash
dusk
```

And that's it. By default it will only change your terminal theme, refer to the configuration section to find out how to do more

## Configuration

### Color schemes
The first change you want to make is probably to use [insert reader's favorite color scheme] which is objectively way better than gruvbox. To change the color scheme you need to head into config.h. Macros starting with L are used when light mode is active, macros starting with D are used for dark mode. If you want a different background color for dark mode, you would need to change DFG. LEMACS and DEMACS are the names of the light and dark emacs color schemes, and LGTKTHEME and DGTKTHEME are the names of the gtk color schemes. DWM is a bit more complicated:
- TEXTBG/FG is for the root window name on the right of the bar and the tiling mode indicator
- BORDER/BORDERSEL is for normal and selected borders
- TAGBG/FG/SELBG/SELFG is for tags, and focused tags respectively
- TITLEBG/FG/SELBG/SELFG is for the window name in the middle of the bar. If you only have one monitor only SEL wil have an effect, if you have multiple monitors the focused one will have SEL colors and the unfocused one/s will have normal BG/FG.

### Turning on more features
You are probaly using more than only your terminal, so you want Dusk to change those program's themes too. As it stands now, Dusk can change the themes of:
- Emacs (when using emacs as a server)
- DWM (with xrdb and dwmc patches)
- GTK
- [Everything pywal can do](https://github.com/dylanaraps/pywal/wiki/Customization)
To turn those on, change the corresponding macro in config.h from 0 to 1.
I can also reccomend the browser extension [Dark Reader](https://darkreader.org/) which changes website themes depending on your GTK theme.
If those are not enough, Dusk was written with hacking in mind. You probably want to look at color-changer.c. If you make something which works well, PRs are welcome.

### Auto switching configuration
There are some options in config.c which change how the automatic switching works
- LIGHT_LOW_TRESH: Dark mode activates when the light value falls below this value.
- LIGHT_HIGH_TRESH: Light mode activates when the light value falls below this value.
- REFRESH_TIME: How often the program should refresh.

### Auto switching activation
Maybe you don't have a webcam, maybe you just made a cool light sensor with an arduino, or maybe you just want the colors to change depending on the time. Whatever your goal is, you don't want to use the webcam to change your colors. That code lies in color-changer.c. Just make a function called lightvalue which returns an int from 0 to 100. There is a hysteresis in dusk.c which makes sure that the screen doesn't rapidly switch between themes when the ambient light is close to a treshold, so you don't have to make that yourselves.

## Contributing
Pull requests are welcome, both to add support for more programs, and to fix my C code which probably violates all sorts of conventions. For major changes, please open an issue first to discuss what you would like to change.

## License
GNU GPL v3
