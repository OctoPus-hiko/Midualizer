# Midualizer
Midualizer is a free 3D&2D MIDI visualizer for Linux (and possibly for other platforms in the future).

## Build Midualizer
Install these dependencies:

- FLTK
- FluidSynth
- GLFW
- GLM
- libepoxy
- Midifile
- range-v3 (C++ library)
- SFML

Then, run the following commands.

`cd midualizer/src`

`meson setup builddir`

`cd builddir`

`meson compile`

`sudo meson install`

If you don't need to install a soundfont, run `meson configure -Dinstall_soundfont=false`.

In this case, you have to put a soundfout in "/usr/share/soundfonts", "/usr/local/share/soundfonts" or "~/.local/share/soundfonts" directory and rename it "default.sf2" to play sound. A symbolic link will do, too.

## Credits
Midualizer relies on these programs among others. I appreciate them very much.
- [FLTK](https://www.fltk.org/)
- [FluidSynth](https://www.fluidsynth.org/)
- [GeneralUser GS soundfont](https://www.schristiancollins.com/generaluser.php)
- [GLFW](https://www.glfw.org)
- [GLM](https://github.com/g-truc/glm)
- [libepoxy](https://github.com/anholt/libepoxy)
- [Midifile](https://midifile.sapp.org/)
- [range-v3](https://github.com/ericniebler/range-v3)
- [SFML](https://www.sfml-dev.org)
- [stb](https://github.com/nothings/stb)

## License
Midualizer is distributed under the MIT License.

See [here]() for the licenses of external programs used by Midualizer.
