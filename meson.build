project('libzakutils', 'c',
        version: '0.0.1')

add_global_link_arguments('-lm', language: 'c')

glib_dep = dependency('glib-2.0')
gobject_dep = dependency('gobject-2.0')
gio_dep = dependency('gio-2.0')

inc = include_directories('src')

subdir('src')
subdir('tests')