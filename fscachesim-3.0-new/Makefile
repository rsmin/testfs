# Makefile.in generated automatically by automake 1.5 from Makefile.am.

# Copyright 1994, 1995, 1996, 1997, 1998, 1999, 2000, 2001
# Free Software Foundation, Inc.
# This Makefile.in is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.



# RCS:         $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/Makefile.am,v 1.19 2002/03/14 22:55:41 tmwong Exp $
# Description: fscachesim Makefile.am
# Created by:  T.M. Wong <tmwong+@cs.cmu.edu>

SHELL = /bin/sh

srcdir = .
top_srcdir = .

prefix = /usr/local
exec_prefix = ${prefix}

bindir = ${exec_prefix}/bin
sbindir = ${exec_prefix}/sbin
libexecdir = ${exec_prefix}/libexec
datadir = ${prefix}/share
sysconfdir = ${prefix}/etc
sharedstatedir = ${prefix}/com
localstatedir = ${prefix}/var
libdir = ${exec_prefix}/lib
infodir = ${prefix}/info
mandir = ${prefix}/man
includedir = ${prefix}/include
oldincludedir = /usr/include
pkgdatadir = $(datadir)/fscachesim
pkglibdir = $(libdir)/fscachesim
pkgincludedir = $(includedir)/fscachesim
top_builddir = .

ACLOCAL = ${SHELL} /home/ol/fscachesim-3.0/config/missing --run aclocal
AUTOCONF = ${SHELL} /home/ol/fscachesim-3.0/config/missing --run autoconf
AUTOMAKE = ${SHELL} /home/ol/fscachesim-3.0/config/missing --run automake
AUTOHEADER = ${SHELL} /home/ol/fscachesim-3.0/config/missing --run autoheader

INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL}
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_SCRIPT = ${INSTALL}
INSTALL_HEADER = $(INSTALL_DATA)
transform = s,x,x,
NORMAL_INSTALL = :
PRE_INSTALL = :
POST_INSTALL = :
NORMAL_UNINSTALL = :
PRE_UNINSTALL = :
POST_UNINSTALL = :
build_alias = 
build_triplet = i686-pc-linux-gnu
host_alias = 
host_triplet = i686-pc-linux-gnu
target_alias = 
target_triplet = i686-pc-linux-gnu
AMTAR = ${SHELL} /home/ol/fscachesim-3.0/config/missing --run tar
AWK = gawk
CC = gcc 
CXX = g++ 
DEPDIR = .deps
EXEEXT = 
INSTALL_STRIP_PROGRAM = ${SHELL} $(install_sh) -c -s
OBJEXT = o
PACKAGE = fscachesim
VERSION = 3.0
am__include = include
am__quote = 
install_sh = /home/ol/fscachesim-3.0/config/install-sh

EXTRA_DIST = reconf

SUBDIRS = doc mambolib scripts

# We do not have _all_ GNU files so add this to avoid warnings
AUTOMAKE_OPTIONS = foreign


# Rules for building fscachesim
bin_PROGRAMS = fscachesim RDMARegSim sharestats tracestats

fscachesim_SOURCES = \
	Block.hh \
	Cache.hh \
	Char.hh \
	Ghost.cc \
	Ghost.hh \
	IORequest.hh \
	IORequestGenerator.hh \
	IORequestGeneratorBatch.cc \
	IORequestGeneratorBatch.hh \
	IORequestGeneratorFile.cc \
	IORequestGeneratorFile.hh \
	IORequestGeneratorFileGeneric.cc \
	IORequestGeneratorFileGeneric.hh \
	IORequestGeneratorFileMambo.cc \
	IORequestGeneratorFileMambo.hh \
	Statistics.hh \
	Store.hh \
	StoreCache.cc \
	StoreCache.hh \
	StoreCacheSLRU.cc \
	StoreCacheSLRU.hh \
	StoreCacheSeg.cc \
	StoreCacheSeg.hh \
	StoreCacheSimple.cc \
	StoreCacheSimple.hh \
	StoreCacheSimpleCoop.cc \
	StoreCacheSimpleCoop.hh \
	StoreCacheLRUCoop.cc \
	StoreCacheLRUCoop.hh \
	StoreCacheSegCoop.cc \
	StoreCacheSegCoop.hh \
	StoreCache2QCoop.cc \
	StoreCache2QCoop.hh \
	StoreCache2SCoop.cc \
	StoreCache2SCoop.hh \
	StoreCache4SCoop.cc \
	StoreCache4SCoop.hh \
	StoreCache2Q.cc \
	StoreCache2Q.hh \
	StoreCacheARC.cc \
	StoreCacheARC.hh \
	StoreCacheMQ.cc \
	StoreCacheMQ.hh \
	2QCache.cc \
	2QCache.hh \
	ARC.cc \
	ARC.hh \
	4QCache.cc \
	4QCache.hh \
	DemoteStat.cc \
	DemoteStat.hh \
	StoreCacheLFUCoop.cc \
	StoreCacheLFUCoop.hh \
	diskActivity.cc \
	diskActivity.hh \
	writeArrayPolicy.cc \
	writeArrayPolicy.hh \
        FreqBlock.hh \
        LFUCache.hh \
        LFUCache.cc \
        util.hh \
        util.cc \
	UInt64.hh \
	fscachesim.cc \
	top-down-size-splay.c \
	top-down-size-splay.h \

RDMARegSim_SOURCES = \
	Block.hh \
	Cache.hh \
	Char.hh \
	Ghost.cc \
	Ghost.hh \
	IORequest.hh \
	IORequestGenerator.hh \
	IORequestGeneratorBatch.cc \
	IORequestGeneratorBatch.hh \
	IORequestGeneratorFile.cc \
	IORequestGeneratorFile.hh \
	IORequestGeneratorFileGeneric.cc \
	IORequestGeneratorFileGeneric.hh \
	IORequestGeneratorFileMambo.cc \
	IORequestGeneratorFileMambo.hh \
	Statistics.hh \
	Store.hh \
	StoreCache.cc \
	StoreCache.hh \
	StoreCacheSLRU.cc \
	StoreCacheSLRU.hh \
	StoreCacheSeg.cc \
	StoreCacheSeg.hh \
	StoreCacheSimple.cc \
	StoreCacheSimple.hh \
	DemoteStat.cc \
	DemoteStat.hh \
        util.hh \
        util.cc \
	UInt64.hh \
	2QCache.cc \
	2QCache.hh \
	2QCache.cc \
	2QCache.hh \
	RDMARegSim.cc\
	StoreCacheLRU2Address.hh\
	StoreCacheLRU2Address.cc\
	ACache.hh\
	top-down-size-splay.c \
	top-down-size-splay.h \
	MemoryRegion.hh \
	RegCache.hh \
	RegCache.cc \
	PindownCache.hh \
	PindownCache.cc \
	PindownBatchDeRegCache.hh \
	PindownBatchDeRegCache.cc \
	PindownResortCache.hh \
	PindownResortCache.cc \
	MRRCache.hh \
	MRRCache.cc \
	MRROptCache.hh \
	MRROptCache.cc \
	MRRResortCache.hh \
	MRRResortCache.cc \
	StoreRegCache.hh \
	StoreRegCache.cc \
        RegSimpleCache.hh \
        RegSimpleCache.cc \

sharestats_SOURCES = \
	Block.hh \
	IORequest.hh \
	IORequestGenerator.hh \
	IORequestGeneratorBatch.cc \
	IORequestGeneratorBatch.hh \
	IORequestGeneratorFile.cc \
	IORequestGeneratorFile.hh \
	IORequestGeneratorFileGeneric.cc \
	IORequestGeneratorFileGeneric.hh \
	IORequestGeneratorFileMambo.cc \
	IORequestGeneratorFileMambo.hh \
	Statistics.hh \
	Store.hh \
	StoreSharestats.cc \
	StoreSharestats.hh \
	sharestats.cc \
	top-down-size-splay.c \
	top-down-size-splay.h


tracestats_SOURCES = \
	Block.hh \
	IORequest.hh \
	IORequestGenerator.hh \
	IORequestGeneratorBatch.cc \
	IORequestGeneratorBatch.hh \
	IORequestGeneratorFile.cc \
	IORequestGeneratorFile.hh \
	IORequestGeneratorFileGeneric.cc \
	IORequestGeneratorFileGeneric.hh \
	IORequestGeneratorFileMambo.cc \
	IORequestGeneratorFileMambo.hh \
	Statistics.hh \
	Store.hh \
	StoreInfinite.cc \
	StoreInfinite.hh \
	top-down-size-splay.c \
	top-down-size-splay.h \
	tracestats.cc

subdir = .
ACLOCAL_M4 = $(top_srcdir)/aclocal.m4
mkinstalldirs = $(SHELL) $(top_srcdir)/config/mkinstalldirs
CONFIG_HEADER = config.h
CONFIG_CLEAN_FILES =
bin_PROGRAMS = fscachesim$(EXEEXT) RDMARegSim$(EXEEXT) sharestats$(EXEEXT) \
	tracestats$(EXEEXT)
PROGRAMS = $(bin_PROGRAMS)

am_fscachesim_OBJECTS = Ghost.$(OBJEXT) \
	IORequestGeneratorBatch.$(OBJEXT) \
	IORequestGeneratorFile.$(OBJEXT) \
	IORequestGeneratorFileGeneric.$(OBJEXT) \
	IORequestGeneratorFileMambo.$(OBJEXT) StoreCache.$(OBJEXT) \
	StoreCacheSLRU.$(OBJEXT) StoreCacheSeg.$(OBJEXT) \
	StoreCacheSimple.$(OBJEXT) StoreCache2Q.$(OBJEXT) StoreCacheMQ.$(OBJEXT)  \
        StoreCacheARC.$(OBJEXT) ARC.$(OBJEXT) StoreCacheSimpleCoop.$(OBJEXT) \
	StoreCacheLRUCoop.$(OBJEXT) StoreCacheSegCoop.$(OBJEXT)  \
        StoreCache2QCoop.$(OBJEXT)  StoreCache2SCoop.$(OBJEXT) 2QCache.$(OBJEXT) \
        4QCache.$(OBJEXT) StoreCache4SCoop.$(OBJEXT)  DemoteStat.$(OBJEXT) fscachesim.$(OBJEXT)  \
        StoreCacheLFUCoop.$(OBJEXT) LFUCache.$(OBJEXT) util.$(OBJEXT) \
        writeArrayPolicy.$(OBJEXT) diskActivity.$(OBJEXT) \
        top-down-size-splay.$(OBJEXT)
fscachesim_OBJECTS = $(am_fscachesim_OBJECTS)
fscachesim_LDADD = $(LDADD)
fscachesim_DEPENDENCIES =
fscachesim_LDFLAGS =

am_RDMARegSim_OBJECTS = Ghost.$(OBJEXT) \
	IORequestGeneratorBatch.$(OBJEXT) \
	IORequestGeneratorFile.$(OBJEXT) \
	IORequestGeneratorFileGeneric.$(OBJEXT) \
	IORequestGeneratorFileMambo.$(OBJEXT) StoreCache.$(OBJEXT) StoreCache2Q.$(OBJEXT)\
	StoreCacheSLRU.$(OBJEXT) StoreCacheSeg.$(OBJEXT) 2QCache.$(OBJEXT)\
	StoreCacheSimple.$(OBJEXT) DemoteStat.$(OBJEXT) RDMARegSim.$(OBJEXT)  \
	util.$(OBJEXT) StoreCacheLRU2Address.$(OBJEXT) RegCache.$(OBJEXT) PindownCache.$(OBJEXT) \
        PindownBatchDeRegCache.$(OBJEXT) MRRCache.$(OBJEXT) top-down-size-splay.$(OBJEXT) \
        StoreRegCache.$(OBJEXT) MRRResortCache.$(OBJEXT) PindownResortCache.$(OBJEXT) \
        MRROptCache.$(OBJEXT) RegSimpleCache.$(OBJEXT)

RDMARegSim_OBJECTS = $(am_RDMARegSim_OBJECTS)
RDMARegSim_LDADD = $(LDADD)
RDMARegSim_DEPENDENCIES =
RDMARegSim_LDFLAGS =


am_sharestats_OBJECTS = IORequestGeneratorBatch.$(OBJEXT) \
	IORequestGeneratorFile.$(OBJEXT) \
	IORequestGeneratorFileGeneric.$(OBJEXT) \
	IORequestGeneratorFileMambo.$(OBJEXT) StoreSharestats.$(OBJEXT) \
	sharestats.$(OBJEXT) top-down-size-splay.$(OBJEXT)
sharestats_OBJECTS = $(am_sharestats_OBJECTS)
sharestats_LDADD = $(LDADD)
sharestats_DEPENDENCIES =
sharestats_LDFLAGS =
am_tracestats_OBJECTS = IORequestGeneratorBatch.$(OBJEXT) \
	IORequestGeneratorFile.$(OBJEXT) \
	IORequestGeneratorFileGeneric.$(OBJEXT) \
	IORequestGeneratorFileMambo.$(OBJEXT) StoreInfinite.$(OBJEXT) \
	top-down-size-splay.$(OBJEXT) tracestats.$(OBJEXT)
tracestats_OBJECTS = $(am_tracestats_OBJECTS)
tracestats_LDADD = $(LDADD)
tracestats_DEPENDENCIES =
tracestats_LDFLAGS =

DEFS = -DHAVE_CONFIG_H
DEFAULT_INCLUDES =  -I. -I$(srcdir) -I.
CPPFLAGS = 
LDFLAGS = 
LIBS = 
depcomp = $(SHELL) $(top_srcdir)/config/depcomp
DEP_FILES = $(DEPDIR)/Ghost.Po \
	$(DEPDIR)/IORequestGeneratorBatch.Po \
	$(DEPDIR)/IORequestGeneratorFile.Po \
	$(DEPDIR)/IORequestGeneratorFileGeneric.Po \
	$(DEPDIR)/IORequestGeneratorFileMambo.Po \
	$(DEPDIR)/StoreCache.Po $(DEPDIR)/StoreCacheSLRU.Po \
	$(DEPDIR)/StoreCacheSeg.Po \
	$(DEPDIR)/StoreCacheSimple.Po \
	$(DEPDIR)/StoreInfinite.Po \
	$(DEPDIR)/StoreSharestats.Po \
	$(DEPDIR)/fscachesim.Po $(DEPDIR)/sharestats.Po \
	$(DEPDIR)/top-down-size-splay.Po \
	$(DEPDIR)/tracestats.Po \
	$(DEPDIR)/StoreCacheSimpleCoop.Po \
	$(DEPDIR)/StoreCacheLRUCoop.Po \
	$(DEPDIR)/StoreCacheSegCoop.Po \
	$(DEPDIR)/StoreCache2QCoop.Po \
	$(DEPDIR)/StoreCache2SCoop.Po \
	$(DEPDIR)/StoreCache4SCoop.Po \
	$(DEPDIR)/2QCache.Po \
	$(DEPDIR)/4QCache.Po \
	$(DEPDIR)/DemoteStat.Po \
        $(DEPDIR)/StoreCacheLFUCoop.Po \
        $(DEPDIR)/LFUCache.Po  \
        $(DEPDIR)/util.Po \
        $(DEPDIR)/StoreCache2Q.Po \
        $(DEPDIR)/StoreCacheMQ.Po \
        $(DEPDIR)/diskActivity.Po \
        $(DEPDIR)/writeArrayPolicy.Po \
	$(DEPDIR)/StoreCacheLRU2Address.Po \
        $(DEPDIR)/PindownResortCache.Po

COMPILE = $(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) \
	$(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)
CCLD = $(CC)
LINK = $(CCLD) $(AM_CFLAGS) $(CFLAGS) $(AM_LDFLAGS) $(LDFLAGS) -o $@
CFLAGS = -g -O2
CXXCOMPILE = $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) \
	$(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS)
CXXLD = $(CXX)
CXXLINK = $(CXXLD) $(AM_CXXFLAGS) $(CXXFLAGS) $(AM_LDFLAGS) $(LDFLAGS) \
	-o $@
CXXFLAGS = -g -O2
DIST_SOURCES = $(fscachesim_SOURCES) $(RDMARegSim_SOURCES) $(sharestats_SOURCES) \
	$(tracestats_SOURCES)

RECURSIVE_TARGETS = info-recursive dvi-recursive install-info-recursive \
	uninstall-info-recursive all-recursive install-data-recursive \
	install-exec-recursive installdirs-recursive install-recursive \
	uninstall-recursive check-recursive installcheck-recursive
DIST_COMMON = ./stamp-h.in Makefile.am Makefile.in aclocal.m4 \
	config.h.in config/acinclude.m4 config/config.guess \
	config/config.sub config/depcomp config/install-sh \
	config/missing config/mkinstalldirs configure configure.in
DIST_SUBDIRS = $(SUBDIRS)
SOURCES = $(fscachesim_SOURCES) $(RDMARegSim_SOURCES) $(sharestats_SOURCES) $(tracestats_SOURCES)

all: config.h
	$(MAKE) $(AM_MAKEFLAGS) all-recursive

.SUFFIXES:
.SUFFIXES: .c .cc .o .obj
$(srcdir)/Makefile.in:  Makefile.am  $(top_srcdir)/configure.in $(ACLOCAL_M4)
	cd $(top_srcdir) && \
	  $(AUTOMAKE) --foreign  Makefile
Makefile:  $(srcdir)/Makefile.in  $(top_builddir)/config.status
	cd $(top_builddir) && \
	  CONFIG_HEADERS= CONFIG_LINKS= \
	  CONFIG_FILES=$@ $(SHELL) ./config.status

$(top_builddir)/config.status: $(srcdir)/configure $(CONFIG_STATUS_DEPENDENCIES)
	$(SHELL) ./config.status --recheck
$(srcdir)/configure:  $(srcdir)/configure.in $(ACLOCAL_M4) $(CONFIGURE_DEPENDENCIES)
	cd $(srcdir) && $(AUTOCONF)

$(ACLOCAL_M4):  configure.in 
	cd $(srcdir) && $(ACLOCAL) $(ACLOCAL_AMFLAGS)
config.h: stamp-h
	@if test ! -f $@; then \
		rm -f stamp-h; \
		$(MAKE) stamp-h; \
	else :; fi
stamp-h: $(srcdir)/config.h.in $(top_builddir)/config.status
	@rm -f stamp-h stamp-hT
	@echo timestamp > stamp-hT 2> /dev/null
	cd $(top_builddir) \
	  && CONFIG_FILES= CONFIG_HEADERS=config.h \
	     $(SHELL) ./config.status
	@mv stamp-hT stamp-h
$(srcdir)/config.h.in:  $(srcdir)/./stamp-h.in
	@if test ! -f $@; then \
		rm -f $(srcdir)/./stamp-h.in; \
		$(MAKE) $(srcdir)/./stamp-h.in; \
	else :; fi
$(srcdir)/./stamp-h.in: $(top_srcdir)/configure.in $(ACLOCAL_M4) 
	@rm -f $(srcdir)/./stamp-h.in $(srcdir)/./stamp-h.inT
	@echo timestamp > $(srcdir)/./stamp-h.inT 2> /dev/null
	cd $(top_srcdir) && $(AUTOHEADER)
	@mv $(srcdir)/./stamp-h.inT $(srcdir)/./stamp-h.in

distclean-hdr:
	-rm -f config.h
install-binPROGRAMS: $(bin_PROGRAMS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(bindir)
	@list='$(bin_PROGRAMS)'; for p in $$list; do \
	  p1=`echo $$p|sed 's/$(EXEEXT)$$//'`; \
	  if test -f $$p \
	  ; then \
	    f=`echo $$p1|sed '$(transform);s/$$/$(EXEEXT)/'`; \
	   echo " $(INSTALL_PROGRAM_ENV) $(INSTALL_PROGRAM) $$p $(DESTDIR)$(bindir)/$$f"; \
	   $(INSTALL_PROGRAM_ENV) $(INSTALL_PROGRAM) $$p $(DESTDIR)$(bindir)/$$f; \
	  else :; fi; \
	done

uninstall-binPROGRAMS:
	@$(NORMAL_UNINSTALL)
	@list='$(bin_PROGRAMS)'; for p in $$list; do \
	  f=`echo $$p|sed 's/$(EXEEXT)$$//;$(transform);s/$$/$(EXEEXT)/'`; \
	  echo " rm -f $(DESTDIR)$(bindir)/$$f"; \
	  rm -f $(DESTDIR)$(bindir)/$$f; \
	done

clean-binPROGRAMS:
	-test -z "$(bin_PROGRAMS)" || rm -f $(bin_PROGRAMS)
fscachesim$(EXEEXT): $(fscachesim_OBJECTS) $(fscachesim_DEPENDENCIES) 
	@rm -f fscachesim$(EXEEXT)
	$(CXXLINK) $(fscachesim_LDFLAGS) $(fscachesim_OBJECTS) $(fscachesim_LDADD) $(LIBS)
RDMARegSim$(EXEEXT): $(RDMARegSim_OBJECTS) $(RDMARegSim_DEPENDENCIES) 
	@rm -f RDMARegSim$(EXEEXT)
	$(CXXLINK) $(RDMARegSim_LDFLAGS) $(RDMARegSim_OBJECTS) $(RDMARegSim_LDADD) $(LIBS)
sharestats$(EXEEXT): $(sharestats_OBJECTS) $(sharestats_DEPENDENCIES) 
	@rm -f sharestats$(EXEEXT)
	$(CXXLINK) $(sharestats_LDFLAGS) $(sharestats_OBJECTS) $(sharestats_LDADD) $(LIBS)
tracestats$(EXEEXT): $(tracestats_OBJECTS) $(tracestats_DEPENDENCIES) 
	@rm -f tracestats$(EXEEXT)
	$(CXXLINK) $(tracestats_LDFLAGS) $(tracestats_OBJECTS) $(tracestats_LDADD) $(LIBS)

mostlyclean-compile:
	-rm -f *.$(OBJEXT) core *.core

distclean-compile:
	-rm -f *.tab.c

include $(DEPDIR)/Ghost.Po
include $(DEPDIR)/IORequestGeneratorBatch.Po
include $(DEPDIR)/IORequestGeneratorFile.Po
include $(DEPDIR)/IORequestGeneratorFileGeneric.Po
include $(DEPDIR)/IORequestGeneratorFileMambo.Po
include $(DEPDIR)/StoreCache.Po
include $(DEPDIR)/StoreCacheSLRU.Po
include $(DEPDIR)/StoreCacheSeg.Po
include $(DEPDIR)/StoreCacheSimple.Po
include $(DEPDIR)/StoreInfinite.Po
include $(DEPDIR)/StoreSharestats.Po
include $(DEPDIR)/fscachesim.Po
include $(DEPDIR)/sharestats.Po
include $(DEPDIR)/top-down-size-splay.Po
include $(DEPDIR)/tracestats.Po
include	$(DEPDIR)/StoreCacheSimpleCoop.Po 
include	$(DEPDIR)/StoreCacheLRUCoop.Po 
include	$(DEPDIR)/StoreCacheSegCoop.Po 
include	$(DEPDIR)/StoreCache2QCoop.Po 
include	$(DEPDIR)/StoreCache2SCoop.Po 
include	$(DEPDIR)/StoreCache4SCoop.Po 
include	$(DEPDIR)/2QCache.Po 
include	$(DEPDIR)/4QCache.Po 
include	$(DEPDIR)/DemoteStat.Po 
include	$(DEPDIR)/StoreCacheLFUCoop.Po 
include	$(DEPDIR)/LFUCache.Po
include	$(DEPDIR)/util.Po
include	$(DEPDIR)/StoreCache2Q.Po
include	$(DEPDIR)/StoreCacheMQ.Po
include	$(DEPDIR)/StoreRegCache.Po
include	$(DEPDIR)/RegCache.Po
include	$(DEPDIR)/PindownCache.Po
include	$(DEPDIR)/PindownBatchDeRegCache.Po
include	$(DEPDIR)/MRRCache.Po
include	$(DEPDIR)/MRRResortCache.Po

distclean-depend:
	-rm -rf $(DEPDIR)

.c.o:
	source='$<' object='$@' libtool=no \
	depfile='$(DEPDIR)/$*.Po' tmpdepfile='$(DEPDIR)/$*.TPo' \
	$(CCDEPMODE) $(depcomp) \
	$(COMPILE) -c `test -f $< || echo '$(srcdir)/'`$<

.c.obj:
	source='$<' object='$@' libtool=no \
	depfile='$(DEPDIR)/$*.Po' tmpdepfile='$(DEPDIR)/$*.TPo' \
	$(CCDEPMODE) $(depcomp) \
	$(COMPILE) -c `cygpath -w $<`
CCDEPMODE = depmode=gcc3

.cc.o:
	source='$<' object='$@' libtool=no \
	depfile='$(DEPDIR)/$*.Po' tmpdepfile='$(DEPDIR)/$*.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXXCOMPILE) -c -o $@ `test -f $< || echo '$(srcdir)/'`$<

.cc.obj:
	source='$<' object='$@' libtool=no \
	depfile='$(DEPDIR)/$*.Po' tmpdepfile='$(DEPDIR)/$*.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXXCOMPILE) -c -o $@ `cygpath -w $<`
CXXDEPMODE = depmode=gcc3
uninstall-info-am:

# This directory's subdirectories are mostly independent; you can cd
# into them and run `make' without going through this Makefile.
# To change the values of `make' variables: instead of editing Makefiles,
# (1) if the variable is set in `config.status', edit `config.status'
#     (which will cause the Makefiles to be regenerated when you run `make');
# (2) otherwise, pass the desired values on the `make' command line.
$(RECURSIVE_TARGETS):
	@set fnord $(MAKEFLAGS); amf=$$2; \
	dot_seen=no; \
	target=`echo $@ | sed s/-recursive//`; \
	list='$(SUBDIRS)'; for subdir in $$list; do \
	  echo "Making $$target in $$subdir"; \
	  if test "$$subdir" = "."; then \
	    dot_seen=yes; \
	    local_target="$$target-am"; \
	  else \
	    local_target="$$target"; \
	  fi; \
	  (cd $$subdir && $(MAKE) $(AM_MAKEFLAGS) $$local_target) \
	   || case "$$amf" in *=*) exit 1;; *k*) fail=yes;; *) exit 1;; esac; \
	done; \
	if test "$$dot_seen" = "no"; then \
	  $(MAKE) $(AM_MAKEFLAGS) "$$target-am" || exit 1; \
	fi; test -z "$$fail"

mostlyclean-recursive clean-recursive distclean-recursive \
maintainer-clean-recursive:
	@set fnord $(MAKEFLAGS); amf=$$2; \
	dot_seen=no; \
	case "$@" in \
	  distclean-* | maintainer-clean-*) list='$(DIST_SUBDIRS)' ;; \
	  *) list='$(SUBDIRS)' ;; \
	esac; \
	rev=''; for subdir in $$list; do \
	  if test "$$subdir" = "."; then :; else \
	    rev="$$subdir $$rev"; \
	  fi; \
	done; \
	rev="$$rev ."; \
	target=`echo $@ | sed s/-recursive//`; \
	for subdir in $$rev; do \
	  echo "Making $$target in $$subdir"; \
	  if test "$$subdir" = "."; then \
	    local_target="$$target-am"; \
	  else \
	    local_target="$$target"; \
	  fi; \
	  (cd $$subdir && $(MAKE) $(AM_MAKEFLAGS) $$local_target) \
	   || case "$$amf" in *=*) exit 1;; *k*) fail=yes;; *) exit 1;; esac; \
	done && test -z "$$fail"
tags-recursive:
	list='$(SUBDIRS)'; for subdir in $$list; do \
	  test "$$subdir" = . || (cd $$subdir && $(MAKE) $(AM_MAKEFLAGS) tags); \
	done

tags: TAGS

ID: $(HEADERS) $(SOURCES) $(LISP) $(TAGS_FILES)
	list='$(SOURCES) $(HEADERS) $(TAGS_FILES)'; \
	unique=`for i in $$list; do \
	    if test -f "$$i"; then echo $$i; else echo $(srcdir)/$$i; fi; \
	  done | \
	  $(AWK) '    { files[$$0] = 1; } \
	       END { for (i in files) print i; }'`; \
	mkid -fID $$unique $(LISP)

TAGS: tags-recursive $(HEADERS) $(SOURCES) config.h.in $(TAGS_DEPENDENCIES) \
		$(TAGS_FILES) $(LISP)
	tags=; \
	here=`pwd`; \
	list='$(SUBDIRS)'; for subdir in $$list; do \
	  if test "$$subdir" = .; then :; else \
	    test -f $$subdir/TAGS && tags="$$tags -i $$here/$$subdir/TAGS"; \
	  fi; \
	done; \
	list='$(SOURCES) $(HEADERS) $(TAGS_FILES)'; \
	unique=`for i in $$list; do \
	    if test -f "$$i"; then echo $$i; else echo $(srcdir)/$$i; fi; \
	  done | \
	  $(AWK) '    { files[$$0] = 1; } \
	       END { for (i in files) print i; }'`; \
	test -z "$(ETAGS_ARGS)config.h.in$$unique$(LISP)$$tags" \
	  || etags $(ETAGS_ARGS) $$tags config.h.in $$unique $(LISP)

GTAGS:
	here=`CDPATH=: && cd $(top_builddir) && pwd` \
	  && cd $(top_srcdir) \
	  && gtags -i $(GTAGS_ARGS) $$here

distclean-tags:
	-rm -f TAGS ID GTAGS GRTAGS GSYMS GPATH

DISTFILES = $(DIST_COMMON) $(DIST_SOURCES) $(TEXINFOS) $(EXTRA_DIST)

top_distdir = .
# Avoid unsightly `./'.
distdir = $(PACKAGE)-$(VERSION)

GZIP_ENV = --best

distdir: $(DISTFILES)
	-chmod -R a+w $(distdir) >/dev/null 2>&1; rm -rf $(distdir)
	mkdir $(distdir)
	$(mkinstalldirs) $(distdir)/config
	@for file in $(DISTFILES); do \
	  if test -f $$file; then d=.; else d=$(srcdir); fi; \
	  dir=`echo "$$file" | sed -e 's,/[^/]*$$,,'`; \
	  if test "$$dir" != "$$file" && test "$$dir" != "."; then \
	    $(mkinstalldirs) "$(distdir)/$$dir"; \
	  fi; \
	  if test -d $$d/$$file; then \
	    cp -pR $$d/$$file $(distdir) \
	    || exit 1; \
	  else \
	    test -f $(distdir)/$$file \
	    || cp -p $$d/$$file $(distdir)/$$file \
	    || exit 1; \
	  fi; \
	done
	for subdir in $(SUBDIRS); do \
	  if test "$$subdir" = .; then :; else \
	    test -d $(distdir)/$$subdir \
	    || mkdir $(distdir)/$$subdir \
	    || exit 1; \
	    (cd $$subdir && \
	      $(MAKE) $(AM_MAKEFLAGS) \
	        top_distdir="$(top_distdir)" \
	        distdir=../$(distdir)/$$subdir \
	        distdir) \
	      || exit 1; \
	  fi; \
	done
	-find $(distdir) -type d ! -perm -777 -exec chmod a+rwx {} \; -o \
	  ! -type d ! -perm -444 -links 1 -exec chmod a+r {} \; -o \
	  ! -type d ! -perm -400 -exec chmod a+r {} \; -o \
	  ! -type d ! -perm -444 -exec $(SHELL) $(install_sh) -c -m a+r {} {} \; \
	|| chmod -R a+r $(distdir)
dist: distdir
	$(AMTAR) chof - $(distdir) | GZIP=$(GZIP_ENV) gzip -c >$(distdir).tar.gz
	-chmod -R a+w $(distdir) >/dev/null 2>&1; rm -rf $(distdir)

# This target untars the dist file and tries a VPATH configuration.  Then
# it guarantees that the distribution is self-contained by making another
# tarfile.
distcheck: dist
	-chmod -R a+w $(distdir) > /dev/null 2>&1; rm -rf $(distdir)
	GZIP=$(GZIP_ENV) gunzip -c $(distdir).tar.gz | $(AMTAR) xf -
	chmod -R a-w $(distdir); chmod a+w $(distdir)
	mkdir $(distdir)/=build
	mkdir $(distdir)/=inst
	chmod a-w $(distdir)
	dc_install_base=`CDPATH=: && cd $(distdir)/=inst && pwd` \
	  && cd $(distdir)/=build \
	  && ../configure --srcdir=.. --prefix=$$dc_install_base \
	  && $(MAKE) $(AM_MAKEFLAGS) \
	  && $(MAKE) $(AM_MAKEFLAGS) dvi \
	  && $(MAKE) $(AM_MAKEFLAGS) check \
	  && $(MAKE) $(AM_MAKEFLAGS) install \
	  && $(MAKE) $(AM_MAKEFLAGS) installcheck \
	  && $(MAKE) $(AM_MAKEFLAGS) uninstall \
	  && (test `find $$dc_install_base -type f -print | wc -l` -le 1 \
	     || (echo "Error: files left after uninstall" 1>&2; \
	         exit 1) ) \
	  && $(MAKE) $(AM_MAKEFLAGS) dist \
	  && $(MAKE) $(AM_MAKEFLAGS) distclean \
	  && rm -f $(distdir).tar.gz \
	  && (test `find . -type f -print | wc -l` -eq 0 \
	     || (echo "Error: files left after distclean" 1>&2; \
	         exit 1) )
	-chmod -R a+w $(distdir) > /dev/null 2>&1; rm -rf $(distdir)
	@echo "$(distdir).tar.gz is ready for distribution" | \
	  sed 'h;s/./=/g;p;x;p;x'
check-am: all-am
check: check-recursive
all-am: Makefile $(PROGRAMS) config.h
installdirs: installdirs-recursive
installdirs-am:
	$(mkinstalldirs) $(DESTDIR)$(bindir)

install: install-recursive
install-exec: install-exec-recursive
install-data: install-data-recursive
uninstall: uninstall-recursive

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am

installcheck: installcheck-recursive
install-strip:
	$(MAKE) $(AM_MAKEFLAGS) INSTALL_PROGRAM="$(INSTALL_STRIP_PROGRAM)" \
	  `test -z '$(STRIP)' || \
	    echo "INSTALL_PROGRAM_ENV=STRIPPROG='$(STRIP)'"` install
mostlyclean-generic:

clean-generic:

distclean-generic:
	-rm -f Makefile $(CONFIG_CLEAN_FILES) stamp-h stamp-h[0-9]*

maintainer-clean-generic:
	@echo "This command is intended for maintainers to use"
	@echo "it deletes files that may require special tools to rebuild."
clean: clean-recursive

clean-am: clean-binPROGRAMS clean-generic mostlyclean-am

dist-all: distdir
	$(AMTAR) chof - $(distdir) | GZIP=$(GZIP_ENV) gzip -c >$(distdir).tar.gz
	-chmod -R a+w $(distdir) >/dev/null 2>&1; rm -rf $(distdir)
distclean: distclean-recursive
	-rm -f config.status config.cache config.log
distclean-am: clean-am distclean-compile distclean-depend \
	distclean-generic distclean-hdr distclean-tags

dvi: dvi-recursive

dvi-am:

info: info-recursive

info-am:

install-data-am:

install-exec-am: install-binPROGRAMS

install-info: install-info-recursive

install-man:

installcheck-am:

maintainer-clean: maintainer-clean-recursive

maintainer-clean-am: distclean-am maintainer-clean-generic

mostlyclean: mostlyclean-recursive

mostlyclean-am: mostlyclean-compile mostlyclean-generic

uninstall-am: uninstall-binPROGRAMS uninstall-info-am

uninstall-info: uninstall-info-recursive

.PHONY: $(RECURSIVE_TARGETS) GTAGS all all-am check check-am clean \
	clean-binPROGRAMS clean-generic clean-recursive dist dist-all \
	distcheck distclean distclean-compile distclean-depend \
	distclean-generic distclean-hdr distclean-recursive \
	distclean-tags distdir dvi dvi-am dvi-recursive info info-am \
	info-recursive install install-am install-binPROGRAMS \
	install-data install-data-am install-data-recursive \
	install-exec install-exec-am install-exec-recursive \
	install-info install-info-am install-info-recursive install-man \
	install-recursive install-strip installcheck installcheck-am \
	installdirs installdirs-am installdirs-recursive \
	maintainer-clean maintainer-clean-generic \
	maintainer-clean-recursive mostlyclean mostlyclean-compile \
	mostlyclean-generic mostlyclean-recursive tags tags-recursive \
	uninstall uninstall-am uninstall-binPROGRAMS uninstall-info-am \
	uninstall-info-recursive uninstall-recursive

# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
