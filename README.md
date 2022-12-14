# <img src="https://octo-translation.com/midualizer/favicon.svg" height="30" alt="Midualizer logo"/> Midualizer

Midualizer is a free 3D&2D MIDI visualizer for Linux.

You can see 3D mode demos on my [YouTube channel](https://www.youtube.com/@octopus-hiko).

## Build Midualizer
First, install these dependencies:

- [FLTK](https://www.fltk.org/)
- [FluidSynth](https://www.fluidsynth.org/)
- [GLFW](https://www.glfw.org)
- [GLM](https://github.com/g-truc/glm)
- [libepoxy](https://github.com/anholt/libepoxy)
- [Midifile](https://midifile.sapp.org/)
- [range-v3](https://github.com/ericniebler/range-v3)
- [SFML](https://www.sfml-dev.org)

Then, get Midualizer's source code from this GitHub page, decompress the compressed file, and run the following commands:

```
cd Midualizer-XXX/src
meson setup builddir
cd builddir
meson compile
sudo meson install
```

If you don't need to install a soundfont, use this configuration option:

`meson configure -Dinstall_soundfont=false`

In this case, to play sound, you have to put a soundfout in any of these directories:
- /usr/share/soundfonts
- /usr/local/share/soundfonts
- ~/.local/share/soundfonts

and rename it "default.sf2". A symbolic link will do, too.

## Credits
Midualizer relies on the above-mentioned dependencies and the following:
- [GeneralUser GS soundfont](https://www.schristiancollins.com/generaluser.php)
- [stb](https://github.com/nothings/stb)

Big thanks to the developers/creators.

## License
Midualizer is distributed under the MIT License.

See [here](https://octo-translation.com/midualizer/external_programs_licenses.html) for the licenses of external programs used by Midualizer.

## Screenshots
<img src="https://octo-translation.com/midualizer/snapshot1.jpg" alt="Midualizer snapshot" height="250" />
<img src="https://octo-translation.com/midualizer/snapshot2.jpg" alt="Midualizer snapshot" height="250" />
<img src="https://octo-translation.com/midualizer/snapshot3.png" alt="Midualizer snapshot" height="250" />
<img src="https://octo-translation.com/midualizer/snapshot4.png" alt="Midualizer snapshot" height="250" />
<img src="https://octo-translation.com/midualizer/snapshot5.png" alt="Midualizer snapshot" height="250" />
