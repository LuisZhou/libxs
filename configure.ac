#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_PREREQ(2.61)
#
# The Crossroads version number is extracted from include/xs.h using
# the version.sh script. Hence, it should be updated there.
# The version in git should reflect the *next* version planned.
#
AC_INIT([libxs],[m4_esyscmd([./version.sh])],[crossroads-dev@groups.crossroads.io])

AC_CONFIG_AUX_DIR(config)
AC_CONFIG_MACRO_DIR(config)
AM_CONFIG_HEADER(src/platform.hpp)
AM_INIT_AUTOMAKE(tar-ustar dist-zip foreign)
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

# This lets us use PACKAGE_VERSION in Makefiles
AC_SUBST(PACKAGE_VERSION)

# ABI version for libxs (Libtool -version-info)
#
# Don't change this unless you know exactly what you're doing and have read and
# understand:
# http://www.gnu.org/software/libtool/manual/html_node/Updating-version-info.html
# This is currently set at ABI 1.0.0.
#
# libxs -version-info current:revision:age
LIBXS_ABI_VERSION="1:0:0"
AC_SUBST(LIBXS_ABI_VERSION)

# ABI version for XS libzmq compatibility library
#
# This must be set to the ABI version of libzmq we are emulating.
# This is currently libzmq 2.1 which is ABI 1.0.0.
#
# libzmq -version-info current:revision:age
LIBZMQ_ABI_VERSION="1:0:0"
AC_SUBST(LIBZMQ_ABI_VERSION)

# Take a copy of original flags
XS_ORIG_CFLAGS="${CFLAGS:-none}"
XS_ORIG_CPPFLAGS="${CPPFLAGS:-none}"
XS_ORIG_CXXFLAGS="${CXXFLAGS:-none}"

# Checks for programs.
AC_PROG_CC
AC_PROG_CC_C99
AC_PROG_CXX
AM_PROG_CC_C_O
AC_PROG_SED
AC_PROG_AWK

# Libtool configuration for different targets. See acinclude.m4
AC_ARG_VAR([XMLTO], [Path to xmlto command])
AC_PATH_PROG([XMLTO], [xmlto])
AC_ARG_VAR([ASCIIDOC], [Path to asciidoc command])
AC_PATH_PROG([ASCIIDOC], [asciidoc])
LIBXS_CONFIG_LIBTOOL
AC_LIBTOOL_WIN32_DLL
AC_PROG_LIBTOOL

# Check whether to build a with debug symbols
LIBXS_CHECK_ENABLE_DEBUG

# Check wheter to enable code coverage
LIBXS_WITH_GCOV

# Checks for libraries.
AC_CHECK_LIB([pthread], [pthread_create])
AC_CHECK_LIB([rt], [clock_gettime])

# Set pedantic
libxs_pedantic="yes"

# By default compiling with -Werror except OSX.
libxs_werror="yes"

# By default use DSO visibility
libxs_dso_visibility="yes"

# Whether we are on mingw or not.
libxs_on_mingw32="no"

# Set some default features required by Crossroads code.
CPPFLAGS="-D_REENTRANT -D_THREAD_SAFE $CPPFLAGS"

# For host type checks
AC_CANONICAL_HOST

# OS-specific tests
case "${host_os}" in
    *linux*)
        # Define on Linux to enable all library features. Define if using a gnu compiler
        if test "x$GXX" = "xyes"; then
            CPPFLAGS="-D_GNU_SOURCE $CPPFLAGS"
        fi
        AC_DEFINE(XS_HAVE_LINUX, 1, [Have Linux OS])

        case "${host_os}" in
            *android*)
                AC_DEFINE(XS_HAVE_ANDROID, 1, [Have Android OS])
            ;;
        esac
        ;;
    *solaris*)
        # Define on Solaris to enable all library features
        CPPFLAGS="-D_PTHREADS $CPPFLAGS"
        AC_DEFINE(XS_HAVE_SOLARIS, 1, [Have Solaris OS])
        AC_CHECK_LIB(socket, socket)
        AC_CHECK_LIB(nsl, gethostbyname)
        AC_MSG_CHECKING([whether atomic operations can be used])
        AC_COMPILE_IFELSE([AC_LANG_PROGRAM(
            [[#include <atomic.h>]],
            [[uint32_t value;
              atomic_cas_32 (&value, 0, 0);
              return 0;]])],
            [solaris_has_atomic=yes],
            [solaris_has_atomic=no])
        AC_MSG_RESULT([$solaris_has_atomic])
        # Solaris 8 does not have atomic operations exported to user space.
        if test "x$solaris_has_atomic" = "xno"; then
            AC_DEFINE(XS_FORCE_MUTEXES, 1, [Force to use mutexes])
        fi
        ;;
    *freebsd*)
        # Define on FreeBSD to enable all library features
        CPPFLAGS="-D__BSD_VISIBLE $CPPFLAGS"
        AC_DEFINE(XS_HAVE_FREEBSD, 1, [Have FreeBSD OS])
        ;;
    *darwin*)
        # Define on Darwin to enable all library features
        CPPFLAGS="-D_DARWIN_C_SOURCE $CPPFLAGS"
        libxs_pedantic="no"
        libxs_werror="no"
        AC_DEFINE(XS_HAVE_OSX, 1, [Have DarwinOSX OS])
        AC_LANG_PUSH([C++])
        LIBXS_CHECK_LANG_FLAG_PREPEND([-Wno-uninitialized])
        AC_LANG_POP([C++])
        ;;
    *netbsd*)
        # Define on NetBSD to enable all library features
        CPPFLAGS="-D_NETBSD_SOURCE $CPPFLAGS"
        AC_DEFINE(XS_HAVE_NETBSD, 1, [Have NetBSD OS])
        # NetBSD 5.0 and newer provides atomic operations but we can
        # only use these on systems where PR #42842 has been fixed so
        # we must try and link a test program using C++.
        libxs_netbsd_has_atomic=no
        AC_MSG_CHECKING([whether atomic operations can be used])
        AC_LANG_PUSH([C++])
        AC_LINK_IFELSE([AC_LANG_PROGRAM(
            [[#include <atomic.h>]],
            [[uint32_t value;
              atomic_cas_32 (&value, 0, 0);
              return 0;]])],
            [libxs_netbsd_has_atomic=yes],
            [libxs_netbsd_has_atomic=no])
        AC_LANG_POP([C++])
        AC_MSG_RESULT([$libxs_netbsd_has_atomic])
        if test "x$libxs_netbsd_has_atomic" = "xno"; then
            AC_DEFINE(XS_FORCE_MUTEXES, 1, [Force to use mutexes])
        fi
        ;;
    *openbsd*)
        # Define on OpenBSD to enable all library features
        CPPFLAGS="-D_BSD_SOURCE $CPPFLAGS"
        AC_DEFINE(XS_HAVE_OPENBSD, 1, [Have OpenBSD OS])
        ;;
    *nto-qnx*)
        libxs_pedantic="no"
        AC_DEFINE(XS_HAVE_QNXNTO, 1, [Have QNX Neutrino OS])
        AC_CHECK_LIB(socket, socket)
        ;;
    *aix*)
        AC_DEFINE(XS_HAVE_AIX, 1, [Have AIX OS])
        ;;
    *hpux*)
        # Define on HP-UX to enable all library features
        CPPFLAGS="-D_POSIX_C_SOURCE=200112L $CPPFLAGS"
        AC_DEFINE(XS_HAVE_HPUX, 1, [Have HPUX OS])
        LIBXS_CHECK_LANG_FLAG_PREPEND([-Ae])
        AC_CHECK_FUNCS(gethrtime)
        ;;
    *mingw32*)
        AC_DEFINE(XS_HAVE_WINDOWS, 1, [Have Windows OS])
        AC_DEFINE(XS_HAVE_MINGW32, 1, [Have MinGW32])
        AC_CHECK_HEADERS(windows.h)
        AC_CHECK_LIB(ws2_32, main, ,
            [AC_MSG_ERROR([cannot link with ws2_32.dll.])])
        AC_CHECK_LIB(rpcrt4, main, ,
            [AC_MSG_ERROR([cannot link with rpcrt4.dll.])])
        AC_CHECK_LIB(iphlpapi, main, ,
            [AC_MSG_ERROR([cannot link with iphlpapi.dll.])])
        # mingw32 defines __int64_t as long long
        AC_LANG_PUSH([C++])
        LIBXS_CHECK_LANG_FLAG_PREPEND([-Wno-long-long])
        AC_LANG_POP([C++])
        libxs_on_mingw32="yes"
        libxs_dso_visibility="no"

        if test "x$enable_static" = "xyes"; then
            AC_MSG_ERROR([Building static libraries is not supported under MinGW32])
        fi
        ;;
    *cygwin*)
        # Define on Cygwin to enable all library features
        CPPFLAGS="-D_GNU_SOURCE $CPPFLAGS"
        AC_DEFINE(XS_HAVE_CYGWIN, 1, [Have Cygwin])
        if test "x$enable_static" = "xyes"; then
            AC_MSG_ERROR([Building static libraries is not supported under Cygwin])
        fi
        ;;
    *)
        AC_MSG_ERROR([unsupported system: ${host_os}.])
        ;;
esac

#
# Check if the compiler supports -fvisibility=hidden flag. MinGW32 uses __declspec
#
if test "x$libxs_dso_visibility" = "xyes"; then
    AC_LANG_PUSH([C++])
    LIBXS_CHECK_LANG_VISIBILITY([LIBXS_EXTRA_CXXFLAGS="$libxs_cv_[]_AC_LANG_ABBREV[]_visibility_flag ${LIBXS_EXTRA_CXXFLAGS}"])
    AC_LANG_POP([C++])
fi

# CPU-specific optimizations
case "${host_cpu}" in
    *sparc*)
        AC_LANG_PUSH([C++])
        LIBXS_CHECK_LANG_FLAG_PREPEND([-mcpu=v9])
        AC_LANG_POP([C++])
    ;;
    *)
    ;;
esac

# Check whether to build docs / install man pages
LIBXS_CHECK_DOC_BUILD

# Check polling system
LIBXS_CHECK_POLLER([CPPFLAGS="${CPPFLAGS} -D${libxs_cv_poller_flag}"],
                    [AC_MSG_ERROR([Unable to continue without polling system])])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS(errno.h arpa/inet.h netinet/tcp.h netinet/in.h stddef.h \
stdlib.h string.h sys/socket.h sys/time.h time.h unistd.h limits.h)

# Check if we have ifaddrs.h header file.
AC_CHECK_HEADERS(ifaddrs.h, [AC_DEFINE(XS_HAVE_IFADDRS, 1, [Have ifaddrs.h header.])])

# Force not to use eventfd
AC_ARG_ENABLE([eventfd], [AS_HELP_STRING([--disable-eventfd], [disable eventfd [default=no]])],
    [xs_disable_eventfd=yes], [xs_disable_eventfd=no])

if test "x$xs_disable_eventfd" != "xyes"; then
    # Check if we have eventfd.h header file.
    AC_CHECK_HEADERS(sys/eventfd.h, [AC_DEFINE(XS_HAVE_EVENTFD, 1, [Have eventfd extension.])])
fi

# Use c++ in subsequent tests
AC_LANG_PUSH(C++)

AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
# Checks for typedefs, structures, and compiler characteristics.
if test "x$libxs_cv_[]_AC_LANG_ABBREV[]_intel_compiler" = "xyes"; then
    dnl 279: controlling expression is constant
    dnl Fixes build with ICC 12.x
    LIBXS_CHECK_WITH_FLAG([-wd279], [AC_TYPE_SIZE_T])
    LIBXS_CHECK_WITH_FLAG([-wd279], [AC_TYPE_SSIZE_T])
else
    AC_TYPE_SIZE_T
    AC_TYPE_SSIZE_T
fi
AC_HEADER_TIME
AC_TYPE_UINT32_T
AC_C_VOLATILE

#  libzmq compatibility mode
libxs_libzmq="no"
AC_ARG_ENABLE([libzmq], [AS_HELP_STRING([--enable-libzmq],
    [install libzmq compatibility library and headers [default=no]])],
    [enable_libzmq=$enableval], [enable_libzmq=no])

if test "x$enable_libzmq" != "xno"; then
    AC_MSG_NOTICE([enabling libzmq compatibility library and headers])
    libxs_libzmq="yes"
fi

#  PGM extension
libxs_pgm_ext="no"

pgm_basename="libpgm-5.1.118~dfsg"

AC_ARG_WITH([pgm], [AS_HELP_STRING([--with-pgm], 
    [build libxs with PGM extension [default=no]])], 
    [with_pgm_ext=$withval], [with_pgm_ext=no])

# build using system pgm
AC_ARG_WITH([system-pgm], [AS_HELP_STRING([--with-system-pgm],
    [build libxs with PGM extension. Requires pkg-config [default=no]])],
    [with_system_pgm_ext=yes], [with_system_pgm_ext=no])

if test "x$with_pgm_ext" != "xno" -a "x$with_system_pgm_ext" != "xno"; then
    AC_MSG_ERROR([--with-pgm and --with-system-pgm cannot be specified together])
fi

if test "x$with_pgm_ext" != "xno"; then

    # This allows placing the tar.gz to foreign/openpgm
    # and using ./configure --with-pgm=libpgm-x.y.z
    if test "x$with_pgm_ext" != "xyes"; then
        pgm_basename="$with_pgm_ext"
    fi

    # Unpack libpgm
    AC_MSG_NOTICE([Unpacking ${pgm_basename}.tar.gz])
    libxs_pwd=`pwd`
    cd foreign/openpgm

    if ! (gzip -dc "${pgm_basename}.tar.gz" || echo "failed") | ${am__untar}; then
        AC_MSG_ERROR([cannot unpack the foreign/openpgm/${pgm_basename}.tar.gz file])
    fi

    cd "${libxs_pwd}"

    if test -d foreign/openpgm/build-staging; then
        rm -rf foreign/openpgm/build-staging
    fi

    mv foreign/openpgm/${pgm_basename} foreign/openpgm/build-staging
    pgm_srcdir=foreign/openpgm/build-staging/openpgm/pgm

    if ! test -d foreign/openpgm/build-staging/openpgm/pgm/config; then
        mkdir foreign/openpgm/build-staging/openpgm/pgm/config
    fi

    # DSO symbol visibility for openpgm
    AC_LANG_PUSH([C])
    LIBXS_CHECK_LANG_VISIBILITY([ac_configure_args="CFLAGS='$libxs_cv_[]_AC_LANG_ABBREV[]_visibility_flag' ${ac_configure_args}"])
    AC_LANG_POP([C])

    pgm_subdir=build-staging/openpgm/pgm
    AC_SUBST(pgm_subdir)

    AC_SUBST(pgm_srcdir)
    AC_CONFIG_SUBDIRS([foreign/openpgm/build-staging/openpgm/pgm/])

    # Success!
    AC_DEFINE(XS_HAVE_OPENPGM, 1, [Have OpenPGM extension])
    libxs_pgm_ext="yes"
fi

# Build with system openpgm
if test "x$with_system_pgm_ext" != "xno"; then
    m4_ifdef([PKG_CHECK_MODULES], [
        PKG_CHECK_MODULES([OpenPGM], [openpgm-5.1 >= 5.1])
        AC_DEFINE(XS_HAVE_OPENPGM, 1, [Have OpenPGM extension])
        LIBXS_EXTRA_CXXFLAGS="$OpenPGM_CFLAGS $LIBXS_EXTRA_CXXFLAGS"
        LIBS="$OpenPGM_LIBS $LIBS"
    ],
    [AC_MSG_ERROR([--with-system-pgm requires a working pkg-config installation])])
fi

AC_SUBST(pgm_basename)

# Set -Wall, -Werror and -pedantic
AC_LANG_PUSH([C++])

# Check how to enable -Wall
LIBXS_LANG_WALL([CPPFLAGS="$libxs_cv_[]_AC_LANG_ABBREV[]_wall_flag $CPPFLAGS"])

if test "x$libxs_werror" = "xyes" -a "x$libxs_cv_[]_AC_LANG_ABBREV[]_sun_studio_compiler" != "xyes"; then
    LIBXS_LANG_WERROR([CPPFLAGS="$libxs_cv_[]_AC_LANG_ABBREV[]_werror_flag $CPPFLAGS"])
fi

if test "x$libxs_pedantic" = "xyes"; then
    LIBXS_LANG_STRICT([CPPFLAGS="$libxs_cv_[]_AC_LANG_ABBREV[]_strict_flag $CPPFLAGS"])
fi
AC_LANG_POP([C++])

AM_CONDITIONAL(BUILD_LIBZMQ, test "x$libxs_libzmq" = "xyes")
AM_CONDITIONAL(BUILD_PGM, test "x$libxs_pgm_ext" = "xyes")
AM_CONDITIONAL(ON_MINGW, test "x$libxs_on_mingw32" = "xyes")

# Checks for library functions.
AC_TYPE_SIGNAL
AC_CHECK_FUNCS(perror gettimeofday clock_gettime memset socket getifaddrs freeifaddrs)
AC_CHECK_HEADERS([alloca.h])
LIBXS_CHECK_SOCK_CLOEXEC([AC_DEFINE(
                              [XS_HAVE_SOCK_CLOEXEC],
                              [1],
                              [Whether SOCK_CLOEXEC is defined and functioning.])
                          ])

# Subst LIBXS_EXTRA_CFLAGS & CXXFLAGS & LDFLAGS
AC_SUBST(LIBXS_EXTRA_CFLAGS)
AC_SUBST(LIBXS_EXTRA_CXXFLAGS)
AC_SUBST(LIBXS_EXTRA_LDFLAGS)

AC_CONFIG_FILES([Makefile src/Makefile doc/Makefile
    perf/Makefile src/libxs.pc \
    builds/msvc/Makefile tests/Makefile \
    foreign/openpgm/Makefile])
if test "x$libxs_libzmq" == "xyes"; then
    AC_CONFIG_FILES([src/libzmq.pc])
fi
AC_OUTPUT

# Print configuration
cat <<EOF | tee -a config.log

## -------------- ##
## Configuration. ##
## -------------- ##

Configuration for $PACKAGE_STRING
    Host OS: $host_os
    Debugging information: ${enable_debug-no}
    GCC code coverage reporting: ${XS_GCOV-no}
    Polling system: $libxs_cv_poller
    Disable eventfd: $xs_disable_eventfd
    Build libzmq compatibility library and headers: $libxs_libzmq
    PGM extension: $with_pgm_ext
    Use system-provided PGM library: $with_system_pgm_ext
    Build documentation: $libxs_build_doc

Environment variables:
    CPPFLAGS: $CPPFLAGS
    CFLAGS: $CFLAGS
    CXXFLAGS: $CXXFLAGS
    LDFLAGS: $LDFLAGS

Installation:
    Prefix: $prefix
    Install man pages: $libxs_install_man

EOF

if test "x$libxs_libzmq" == "xyes"; then
    AC_MSG_WARN([The --enable-libzmq option was specified:])
    AC_MSG_WARN([libzmq compatibility library and headers are enabled.])
    AC_MSG_WARN([This will overwrite any existing installation of libzmq.])
fi

