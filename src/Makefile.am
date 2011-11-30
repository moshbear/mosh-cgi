## @(#) Makefile.am - Automake file for the FastCGI++ src directory
##
## $Id$
##

DISTCLEANFILES = Makefile.in Makefile

include $(top_srcdir)/include/headerlist

INCLUDES = -I$(top_srcdir)/include
LDADD = $(MOSH_FCGI_LIBS)

lib_LTLIBRARIES = libmosh_cgi.la

libmosh_cgi_la_LDFLAGS = -version-info 0:3:0

libmosh_cgi_la_SOURCES = $(HEADER_LIST) \
	http_misc.cpp
