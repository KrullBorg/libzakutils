AM_CPPFLAGS = $(WARN_CFLAGS) \
              $(DISABLE_DEPRECATED_CFLAGS) \
              $(ZAKUTILS_CFLAGS) \
              -I$(top_srcdir)/src

LIBS = $(ZAKUTILS_LIBS) \
       -export-dynamic

LDADD = $(top_builddir)/src/libzakutils.la -lm

noinst_PROGRAMS = \
                  format_money \
                  gdatetime_from_string
