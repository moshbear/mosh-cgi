# Makefile input to be processed by sed, perl, or other basic regex processor.
# Use it to create subdir Makefiles.
#
# Key to __rexf__(A-Z_])__ vars:
# \1 == CC => C compiler
# \1 == CXX => C++ compiler
# \1 == CFLAGS => C compiler flags
# \1 == CXXFLAGS => C++ compiler flags
# \1 == CCINC => C include dirs
# \1 == CXXINC => C++ include dirs
# \1 == AR => ar
# \1 == RANLIB => ranlib
# \1 == SRCS => sources
# \1 == HDRS => headers
# \1 == UNIT => lib name
# \1 == ifHDRS => expands to '$(addprefix out/, $(addsuffix -stamp, $(HEADERS)))' if HDRS is non-empty
# \1 == ifSRCS => epands to '$(LIBS)' if SRCS is non-empty

CC = gcc -Wall -Wextra
CXX = g++ -Wall -Wextra -std=c++0x
CFLAGS = 
ifndef NO_INHERIT_CFLAGS
CXXFLAGS = $(CFLAGS) 
else
CXXFLAGS = 
endif
CCINCLUDE = 
ifndef NO_INHERIT_CCINCLUDE
CXXINCLUDE = $(CCINCLUDE) -I. -I.. -I../..
else
CXXINCLUDE = -I. -I.. -I../..
endif
LD_MERGE = ld -r
AR = ar
AR_MERGE = ./ar-merge
RANLIB = ranlib

SUBDIRS = form utils
HEADERS = Cgi.h Environment.h Exceptions.h 

LIBS = $(addprefix libcgi, .so .a)

all: $(addprefix out/, $(addsuffix -stamp, $(HEADERS))) $(LIBS)

libcgi.a: form/libcgi-form.a utils/libcgi-utils.a | out
	$(AR_MERGE) $@ $^

libcgi.so: form/libcgi-form.so utils/libcgi-utils.so | out
	$(LD_MERGE) -dy -pie -o $@ $^

out/%.h-stamp: %.h | out
	$(CXX) $(CXXFLAGS) $(CXXINCLUDE) $<
	touch $@

out:
	mkdir $@

clean:
	rm -f out/*
	rm -f *.gch
	rm -f $(LIBS)
