LIBS = $(ZAKUTILS_LIBS)

AM_CPPFLAGS = $(ZAKUTILS_CFLAGS) \
              -DLOCALEDIR=\"$(localedir)\" \
              -DG_LOG_DOMAIN=\"ZakUtils\"

lib_LTLIBRARIES = libzakutils.la

libzakutils_la_SOURCES = \
                         generic.c \
                         datetime.c

libzakutils_la_LDFLAGS = -no-undefined

libzakutils_include_HEADERS = \
                              libzakutils.h \
                              generic.h \
                              datetime.h

libzakutils_includedir = $(includedir)/libzakutils
