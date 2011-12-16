## @(#) Makefile.am - Automake file for the FastCGI++ src directory
##
## $Id$
##

DISTCLEANFILES = Makefile.in Makefile

include $(top_srcdir)/include/headerlist

INCLUDES = -I$(top_srcdir)/include
LDADD = $(MOSH_FCGI_LIBS)

CXXFLAGS = -g -ggdb -O0

lib_LTLIBRARIES = libmosh_cgi.la

libmosh_cgi_la_LDFLAGS = -version-info 0:3:0

libmosh_cgi_la_SOURCES = $(HEADER_LIST) \
	cookie.cpp \
	html_doctype.cpp \
	http_misc.cpp \
	header_helper/content_type.cpp \
	header_helper/redirect.cpp \
	header_helper/response.cpp \
	header_helper/status.cpp \
	header_helper/status_helper.cpp
