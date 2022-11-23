project('midualizer', 'cpp', version: '4.1.0', default_options: 'cpp_std=c++20')

src = ['2d.cpp', 'attributes.cpp', 'fountain.cpp', 'generate_wav.cpp', 'gui_constructor.cpp', 'gui_functions.cpp', 'load_image.cpp', 'main.cpp', 'notes.cpp', 'prism.cpp', 'shaders.cpp', 'trackcolor.cpp', 'trackcount.cpp']

if get_option('flatpak')
    deps = [dependency('epoxy'), dependency('glfw3'), dependency('opengl'), dependency('fluidsynth')]
    links = ['-lfltk', '-lmidifile', '-lsfml-system', '-lsfml-audio']
else
    deps = [dependency('epoxy'), dependency('glfw3'), dependency('opengl'), dependency('fluidsynth'), dependency('sfml-audio')]
    links = ['-lfltk', '-lmidifile']
endif

datadir = get_option('datadir')

if get_option('install_soundfont')
    install_data('GeneralUser_GS_v1.471.sf2', install_dir: datadir / 'soundfonts')
endif

install_data('com.octo_translation.Midualizer.desktop', install_dir: datadir / 'applications')
install_data('com.octo_translation.Midualizer.svg', install_dir: datadir / 'icons/hicolor/scalable/apps')

executable('midualizer', sources: src, dependencies: deps, link_args: links, install_dir: get_option('bindir'), install: true)
