project('libzakutils', 'c',
        version: '0.0.1')

add_global_link_arguments('-lm', language: 'c')

glib_dep = dependency('glib-2.0')
gobject_dep = dependency('gobject-2.0')
gio_dep = dependency('gio-2.0')

inc = include_directories('src')

subdir('src')
subdir('tests')

pkg_mod = import('pkgconfig')
pkg_mod.generate(libraries: libzakutils,
                 version: '0.0.1',
                 name: 'libzakutils',
                 filebase: 'libzakutils',
                 description: 'Utility functions.',
                 requires: ['glib-2.0 >= 2.36', 'gobject-2.0 >= 2.36', 'gio-2.0 >= 2.36'])