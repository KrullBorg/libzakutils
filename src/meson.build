install_headers(['libzakutils.h', 'datetime.h', 'generic.h'],
                subdir: 'libzakutils')

libzakutils_sources = ['datetime.c', 'generic.c']

libzakutils = shared_library ('zakutils',
                              libzakutils_sources,
                              dependencies: [glib_dep, gobject_dep, gio_dep, m_dep],
                              install: true)