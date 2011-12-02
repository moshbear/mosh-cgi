SUBDIRS = include src

DISTCLEANFILES = Makefile Makefile.in

EXTRA_DIST = lgpl-3.0.txt

nobase_dist_pkgdata_DATA = doc/*

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = mosh_cgi.pc

doc/*:
	doxygen doxygen	

