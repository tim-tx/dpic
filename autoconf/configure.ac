dnl Process this file with autoconf to produce a configure script.
dnl i.e. autoconf configure.ac > ../Makefile

AC_INIT(dpic,2018.04.03)
AC_CONFIG_AUX_DIR(autoconf/scripts)
m4_include(m4/ax_check_compile_flag.m4)

AC_LANG_C

PACKAGE=dpic
UCPACKAGE=DPIC
VERSION=2018.04.03
AC_SUBST(PACKAGE)
AC_SUBST(VERSION)
AC_SUBST(UCPACKAGE)
AC_DEFINE_UNQUOTED(PACKAGE, "$PACKAGE")
AC_DEFINE_UNQUOTED(PROGRAM_NAME, "$PACKAGE")
AC_DEFINE_UNQUOTED(VERSION, "$VERSION")

AC_ARG_ENABLE(safemode,
  [  --enable-safemode      compile with sh and copy disabled],
  if test "$enable_safemode" = "yes"; then
    AC_SUBST(safe_mode,-DSAFE_MODE)
  fi
)

AC_CHECK_PROGS(PC,[fpc gpc])

if test "$ac_cv_prog_PC" == "gpc"
then
  AC_SUBST(INC,H)
  AC_SUBST(OBJECTS,dpic.o)
  AC_SUBST(MAKETARGET,['$(PC) -o $(DDIR)/dpic $(OBJECTS)'])
  AC_SUBST(SED1)
else
  AC_SUBST(INC,F)
  AC_SUBST(OBJECTS,dpic)
  AC_SUBST(MAKETARGET,['mv dpic $(DDIR)'])
  AC_SUBST(SED1,['$(SEDF)'])
fi

CFLAGS=${CFLAGS:-"-O"}
AC_PROG_CC

dnl AX_CHECK_COMPILE_FLAG([-std=c90],[AC_SUBST(cninety,-std=c90)])

AC_PROG_INSTALL
AC_PROG_MAKE_SET

AC_CHECK_HEADER(inttypes.h,
  AC_SUBST(def_intt,-DHAVE_INTTYPES_H),
  AC_SUBST(no_intt,-DNO_INTTYPES_H))
AC_CHECK_HEADERS([time.h math.h assert.h])

AC_CHECK_FUNC(random,,
  [AC_CHECK_LIB(iberty,random,
   AC_SUBST(iberty,-liberty),AC_SUBST(defrand,-DRAND))])

AC_CHECK_FUNC(snprintf,,[AC_SUBST(defno_snprintf,-DNO_SNPRINTF)])

test -z "$INSTALL_DATA" && INSTALL_DATA='${INSTALL} -m 644'
AC_SUBST(INSTALL_DATA)

AC_OUTPUT(Makefile
          src/pascal/Makefile:src/pascal/Makefile.in
          src/p2c/src/Makefile:src/p2c/src/Makefile.in
          src/Pgen/src/Makefile:src/Pgen/src/Makefile.in)

dnl EOF
