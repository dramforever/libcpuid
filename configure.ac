AC_INIT([libcpuid CPU Identification library], [0.7.1], [libcpuid-devel@lists.sourceforge.net], [libcpuid])
AC_CONFIG_SRCDIR([libcpuid/libcpuid.h])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([-Wall dist-bzip2 dist-zip foreign])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

dnl Versioning scheme shamelessly copied from libexif.
dnl Short walkthrough. C means CURRENT, A mean AGE, R means REVISION
dnl 1) When you make a change that breaks binary compatibility, increment CURRENT, reset REVISION to 0 and:
dnl   1.1) If the change doesn't remove or change interfaces, increment AGE, otherwise reset to 0.
dnl 2) When you make just a source change that doesn't break binary compatibility, increment REVISION.
dnl When you make a change that adds features, increment AGE, and set REVISION to 0
dnl C:A:R
dnl 10:0:0   Version 0.1.0
dnl 11:0:0   Version 0.1.1: four more fields to cpu_raw_data_t
dnl 11:0:1   Version 0.1.2: added cpu_msr* functions
dnl 11:0:2   Version 0.1.3: added cpu_clock_by_ic() function
dnl 11:0:3   Version 0.1.4: just an identification change, to reflect the detection table upgrades
dnl 12:1:0   Version 0.2.0: two more fields to cpu_id_t
dnl 12:1:1   Version 0.2.1: more processors support
dnl 12:1:2   Version 0.2.2: more processors support, *BSD/Solaris support, updates to MSR stuff
dnl 13:0:0   Version 0.3.0: increment max Intel leaf 04 entries to 8
dnl 14:0:0   Version 0.4.0: add one more field per cpu_raw_data_t and cpu_id_t
dnl 14:0:1   Version 0.4.1: DB updates, and a lot of rdmsr improvements
dnl 15:0:0   Version 0.5.0: add one more field per cpu_raw_data_t (amd_fn8000001dh) and cpu_id_t l1_{data,instruction}_{assoc,cacheline}
dnl 15:0:1   Version 0.5.1: DB updates
dnl 16:0:0   Version 0.6.0: add more fields in cpu_id_t (cache instances), support for hybrid CPUs, DB updates
dnl 16:0:1   Version 0.6.1: DB updates, fixes
dnl 16:0:2   Version 0.6.2: DB updates, fix for single-core CPUs
dnl 16:0:3   Version 0.6.3: DB updates, fixes, restore CPU affinity, rename {set,get}_error() to cpuid_{set,get}_error()
dnl 16:0:4   Version 0.6.4: DB updates, fixes, support for Centaur CPUs (VIA and Zhaoxin)
dnl 16:0:5   Version 0.6.5: DB updates, various improvements, detection of LP E-Cores for Intel CPUs
dnl 17:0:0   Version 0.7.0: DB updates, fixes, various improvements, add cpu_clock_by_tsc() function, add support for ARM CPUs, add cpu_feature_level_t enumerated values, add more fields in cpu_raw_data_t (amd_fn80000026h, arm_*)
dnl 17:0:1   Version 0.7.1: DB updates, fixes
LIBCPUID_CURRENT=17
LIBCPUID_AGE=0
LIBCPUID_REVISION=1
AC_SUBST([LIBCPUID_AGE])
AC_SUBST([LIBCPUID_REVISION])
AC_SUBST([LIBCPUID_CURRENT])
AC_SUBST([LIBCPUID_CURRENT_MIN],[`expr $LIBCPUID_CURRENT - $LIBCPUID_AGE`])
LIBCPUID_VERSION_INFO="$LIBCPUID_CURRENT:$LIBCPUID_REVISION:$LIBCPUID_AGE"
AC_SUBST([LIBCPUID_VERSION_INFO])


AC_PROG_CC
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])
AC_C_CONST
LT_INIT
AM_CPPFLAGS="$CPPFLAGS"

AC_CHECK_HEADERS([stdint.h])
AC_CHECK_FUNCS([popcount64])

AC_CHECK_PROGS([DOXYGEN], [doxygen])
AM_CONDITIONAL([HAVE_DOXYGEN], [test -n "$DOXYGEN"])

AM_LDFLAGS="$LDFLAGS"
if test "x$GCC" = "xyes"; then
    AM_CFLAGS="$AM_CFLAGS -ansi -pedantic-error"
    AM_CXXFLAGS="$AM_CXXFLAGS -ansi -pedantic-error"
    AM_CPPFLAGS="$AM_CPPFLAGS -g -Wall -Wmissing-declarations -Wmissing-prototypes"
    AM_LDFLAGS="$AM_LDFLAGS -g -Wall"
fi

AC_CANONICAL_HOST

build_windows=no
build_netbsd=no
build_dragonflybsd=no
build_freebsd=no
build_linux=no

case "${host_os}" in
    cygwin*|mingw*)
        build_windows=yes
        ;;
    netbsd*)
        build_netbsd=yes
        ;;
    dragonfly*)
        build_dragonflybsd=yes
        ;;
    freebsd*)
        build_freebsd=yes
        ;;
    linux*)
        build_linux=yes
        ;;
esac

if test "$build_windows" = "no"; then
    AM_CPPFLAGS="$AM_CPPFLAGS -D_GNU_SOURCE"
fi

if test "$build_netbsd" = "yes" || test "$build_dragonflybsd" = "yes"; then
    AM_LDFLAGS="$AM_LDFLAGS -pthread"
fi

if test "$build_linux" = "yes"; then
    AC_CHECK_FUNCS([getauxval])
fi

if test "$build_freebsd" = "yes"; then
    AC_CHECK_FUNCS([elf_aux_info])
fi

AM_CONDITIONAL([WINDOWS], [test "$build_windows" = "yes"])

AC_SUBST(AM_CPPFLAGS)
AC_SUBST(AM_LDFLAGS)


AC_CONFIG_FILES([
  Makefile
  libcpuid.pc
  libcpuid/Makefile
  cpuid_tool/Makefile
  tests/Makefile
  libcpuid/Doxyfile
])
AC_OUTPUT
