DEFS = @DEFS@

sbin_PROGRAMS = dependencia

dependencia_SOURCES = discover_core.c

CPPFLAGS = @CPPFLAGS@ -I./include -DCONFIG_FILE="\"@sysconfdir@/dependencia_config.xml\""
LDFLAGS = @LDFLAGS@
#SUBDIRS = database 
AUTOMAKE_OPTIONS = no-dependencies
dependencia_CPPFLAGS = @CPPFLAGS@ -DPIDFILE="\"@localstatedir@/run/dependencia.pid\""
EXTRA_DIST = dependencia.spec install-sh

install-data-hook:
	sed -e "s:LIBDIR:$(libdir):" \
	    -e "s:DATAROOTDIR:$(datarootdir):" \
	    -e "s:LIBEXECDIR:$(libexecdir):" \
	dependencia_config.xml.in > dependencia_config.xml

	$(MKDIR_P) $(DESTDIR)$(sysconfdir)
	if test -e $(DESTDIR)$(sysconfdir)/dependencia_config.xml; then \
		$(INSTALL_DATA) $(srcdir)/dependencia_config.xml $(DESTDIR)$(sysconfdir)/dependencia_config.xml.new; \
	else \
		$(INSTALL_DATA) $(srcdir)/dependencia_config.xml $(DESTDIR)$(sysconfdir)/dependencia_config.xml; \
	fi

