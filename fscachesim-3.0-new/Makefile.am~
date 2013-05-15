# RCS:         $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/Makefile.am,v 1.19 2002/03/14 22:55:41 tmwong Exp $
# Description: fscachesim Makefile.am
# Created by:  T.M. Wong <tmwong+@cs.cmu.edu>

EXTRA_DIST = reconf

SUBDIRS = doc mambolib scripts

# We do not have _all_ GNU files so add this to avoid warnings
AUTOMAKE_OPTIONS = foreign

# Rules for building fscachesim

bin_PROGRAMS = fscachesim sharestats tracestats

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
	UInt64.hh \
	fscachesim.cc \
	top-down-size-splay.c \
	top-down-size-splay.h


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
