AC_INIT(src/Makevars.in)

AC_PATH_PROG(PKG_CONFIG, pkg-config)

ARROW_CFLAGS=`$PKG_CONFIG --cflags arrow`
ARROW_LDFLAGS=`$PKG_CONFIG --libs arrow`

BOOST_LIBS="-lboost_system -lboost_filesystem -Wl,-rpath,/usr/local/lib"

AC_SUBST(ARROW_CFLAGS)
AC_SUBST(ARROW_LDFLAGS)
AC_SUBST(BOOST_LIBS)

AC_OUTPUT(src/Makevars)
