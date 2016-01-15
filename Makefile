# Installation shortcuts: change the following and type "make install"

# For Linux (expects "make DESTDIR=xxx PREFIX=yyy install"):
DESTDIR = $${HOME}
PREFIX = .
DEST = ${DESTDIR}/${PREFIX}/bin

# For manual installation (uncomment and change as necessary):
# DEST = $${HOME}/bin

#-----------------------------------------------------------------------

# System-dependent parameters.  These may need tweaking:

# Uncomment this to force the use of rand() and srand() respectively instead of
# random() and srandom(). This is necessary for some Linux systems.
# DEFINEA=-DRAND

# If snprintf is unavailable then uncomment the following, type "make clean",
# and try again:
# DEFINEB=-DNO_SNPRINTF

# Server operation: Use the -z option or uncomment the following to compile
# with read and write access (sh and copy) to arbitrary files disabled.
# SAFEMODE= -DSAFE_MODE

# For DJGPP compilation:
# DEFINEB=
# CFLAGS= $(DEFINEA) -DNO_SNPRINTF $(SAFEMODE) -mcpu=pentium -march=i386 -O

# MinGW
# LIBS = -liberty

# Ultrix mips
# CFLAGS = $(DEFINEA) $(DEFINEB) $(SAFEMODE) -O -Olimit 1600

# AIX
# CC=xlc

# other.  cc usually works too.
CFLAGS += $(DEFINEA) $(DEFINEB) $(SAFEMODE) -O
CC=gcc

OBJECTS = dpic.o p2clib.o

# Uncomment to compile debug code:
# DEBUG = -e 's/(\*D\([A-Z]\)/(*\1/g' -e 's/\([A-Z]\)D\*)/\1*)/g' \
#    | sed -e 's/(\*D//g' -e 's/D\*)//g'
# OBJECTS = dpic.o p2clib.o sysdep.o

#-----------------------------------------------------------------------

SOURCES = \
 src/dp0.x src/dpic.x src/dpic1.x src/mfp.x \
 src/mpo.x src/pdf.x src/pgf.x src/ps.x src/pst.x src/svg.x \
 src/sysdep.x src/tex.x src/xfig.x \
 src/dpic.bnf

dpic: dpic.c $(OBJECTS)
	$(CC) -o dpic $(OBJECTS) -lm $(LIBS)

LEXCINCLUDES = entryhp.h lxhp.h lxnp.h lxtv.h entrytv.h lxch.h
CMPRT = awk '{b=b$$0; if(length(b) > 75){print b; b=""}}; END{print b}'
dpic.c: $(SOURCES)
	(cd src; make DEBUG="$(DEBUG)" )
	cp src/dpic.c src/parstab.i .
	for name in $(LEXCINCLUDES) ; do \
       $(CMPRT) src/$$name > $$name ; done;
	chmod 644 *.c *.i *.h
	if test -n "$(DEBUG)" ; \
      then $(CC) -c src/sysdep.c; fi

install: dpic
	mkdir -p $(DEST)
	if test -x dpic.exe ; then \
      strip dpic.exe; install dpic.exe $(DEST) ; \
    else \
      strip dpic; install -m 755 dpic $(DEST) ; \
    fi
#                               Delete compiled files
clean:
	-rm -f *.o dpic dpic.exe *.LOG
	(cd src; make clean )

distclean:
	-rm -f *.o dpic dpic.exe *.LOG
	(cd src; make veryclean)
	(cd doc; make clean)
	find . -name \*stackdump -exec rm {} \;

#                               Delete computed files in this and lower folders
veryclean: clean
	find . -name \*stackdump -exec rm {} \;
	rm -f dpic.c lx*.h ent*.h *.i sysdep.c
	(cd src; make veryclean)
	(cd doc; make clean)
