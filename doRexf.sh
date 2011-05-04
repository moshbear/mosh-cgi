#!/bin/bash

# $1 = subdir for which to generate and place Makefile

# use ENV form for CC, CXX,
#                  CFLAGS, CXXFLAGS,
#                  CCINCLUDE, CXXINCLUDE,
#                  AR, RANLIB

if [ "$1" = "-h" -o "$1" == "--help" ]; then
	cat << "EOF"
doRexf.sh [dirname [libname]]
EOF
exit
fi

if [ -z "$PROJDIR" ]; then
PROJDIR="$(pwd)"
fi

MYDIR="${1:-.}"
shift;
if [ -n "$1" ]; then
MYLIBNAME="$1"
else
if [ "$MYDIR" != "." ]; then
MYLIBNAME="$MYDIR"
else
MYLIBNAME=""
fi
fi

MYREXF="${REXF:-sed}"

# get the SOURCES and HEADERS from files
# newline is converted to space to allow line-independent parsing
get_srcs() {
	cat "$MYDIR"/SOURCES | tr '\n' ' '
}
get_hdrs() {
	cat "$MYDIR"/HEADERS | tr '\n' ' '
}

# generate Makefile rules to all if non-empty corresponding SOURCES/HEADERS
# order is reversed from get_* so that `make` builds headers first, then libs
if_hdrs() {
	UN="$1"
	if [ -n "$(get_hdrs|tr -d '[ \t]')" ]; then
		echo -n "../$UN.h"
	else
		echo -n
	fi
}
if_srcs() {
	if [ -n "$(get_srcs|tr -d '[ \t]')" ]; then
		echo -n '$(LIBS)'
	else
		echo -n
	fi
}
add_leading_hyphen_if_n() {
	[ -n "$1" ] && echo "-$1" || echo "$1";
}

# Note: ';' is probably the only safe s/// delimiter to use because the other
# ones might be valid (especially : and /)
$MYREXF -e \
"s;__rexf__CC__;${CC:-gcc -Wall -Wextra};g
s;__rexf__CXX__;${CXX:-g++ -Wall -Wextra -std=c++0x};g
s;__rexf__CFLAGS__;$CFLAGS;g
s;__rexf__CXXFLAGS__;$CXXFLAGS;g
s;__rexf__CCINC__;$CCINCLUDE;g
s;__rexf__CXXINC__;${CXXINCLUDE:--I. -I.. -I../.. -I../../stxxl-1.3.1/include/};g
s;__rexf__AR__;${AR:-ar};g
s;__rexf__RANLIB__;${RANLIB:-ranlib};g
s;__rexf__SRCS__;$(get_srcs);g
s;__rexf__HDRS__;$(get_hdrs);g
s;__rexf__UNIT__;$MYLIBNAME;g
s;__rexf__UNITL__;$(add_leading_hyphen_if_n $MYLIBNAME);g
s;__rexf__MKHDRS__;\"$PROJDIR\"/mk-headers;g
s;__rexf__ifHDRS__;$(if_hdrs $MYLIBNAME);g
s;__rexf__ifSRCS__;$(if_srcs);g" \
__rexf__Makefile > "$MYDIR"/Makefile
