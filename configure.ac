AC_INIT([mosh_cgi], [0.3.2], [m0shbear <andrey@moshbear.net>], [mosh_cgi])
AC_CONFIG_FILES(mosh_cgi.pc)
AC_CONFIG_HEADERS([include/mosh/cgi/bits/config.h])
AM_INIT_AUTOMAKE(1.9 dist-bzip2 no-dist-gzip tar-ustar foreign no-dependencies)

AC_LANG(C++)
AC_REQUIRE_CPP

AC_PROG_INSTALL
AC_PROG_LIBTOOL

PKG_CHECK_MODULES([MOSH_FCGI], [mosh_fcgi], , AC_MSG_ERROR([cannot find mosh_fcgi]))
AC_SUBST([MOSH_FCGI_CFLAGS], [$MOSH_FGCI_CFLAGS])
AC_SUBST([MOSH_FCGI_LIBS], [$MOSH_FCGI_LIBS])

AC_PROG_CXX

CXX="${CXX} -std=c++0x -Wall"

AC_MSG_NOTICE([checking for language features])
AC_COMPILE_CHECK([c++11 angle brackets], [ template <typename T> class Q { }; ] , [ Q<Q<int>> foo; ], [AC_MSG_NOTICE([working])], [AC_MSG_ERROR([no improved angle bracket support])])
AC_COMPILE_CHECK([c++11 auto], , [ int foo; auto bar = foo; ], [AC_MSG_NOTICE([working])], [AC_MSG_ERROR([no auto support])])
AC_COMPILE_CHECK([c++11 rvalue reference], [int p() { return 0; } ] , [ int&& q = p(); ], [AC_MSG_NOTICE([working])], [AC_MSG_ERROR([no rvalue reference support])])
AC_COMPILE_CHECK([c++11 lambda], , [ auto&& foo = [] (int x) { return x; }; ], [AC_MSG_NOTICE([working])], [AC_MSG_ERROR([no lambda support])])
AC_COMPILE_CHECK([c++11 array], [#include <array> ], [std::array<int, 1> x; ], [AC_MSG_NOTICE([working])],  [AC_MSG_ERROR([no c++11 array])])
dnl todo: rb-for workaround where applicable
AC_COMPILE_CHECK([c++11 range-based for], , [ int foo[2] = {0, 1}; for (auto& bar : foo); ], [AC_MSG_NOTICE([working])], [AC_MSG_ERROR([no range-based for support])])
AC_COMPILE_CHECK([c++11 smart pointer], [#include <memory>], [ std::unique_ptr<void> p; std::shared_ptr<void> q; ], [AC_MSG_NOTICE([working])],[AC_MSG_ERROR([no unique_ptr])])
AC_COMPILE_CHECK([c++11 <mutex>::call_once], [#include <mutex>], [ std::once_flag o; ], [AC_MSG_NOTICE([working])], [AC_MSG_ERROR([no <mutex>])])

pkgConfigLibs="-lmosh_fcgi $MOSH_FCGI_LIBS"

AC_SUBST(pkgConfigLibs)

AC_OUTPUT([Makefile \
	src/Makefile \
	include/Makefile ])
