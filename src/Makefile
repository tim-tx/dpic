# Assemble pascal code, generate ANSI C and include files

#                               Compile p2c locally
P2C = p2c/p2c
#                               Uncomment if p2c is installed
#P2C = p2c

# Debug off
DEBUG=

# Gcc:
OPTS= -O
CC=gcc

DATE = `date +%Y.%m.%d`
# DATE = 2014.01.01

CPP= cpp -E -P

# --------------------------------------------------------------------- #

INC=M
#                               Compiled tables
COMPTABLES= -e 's/P2 IP/P2CIP/' -e 's/P2 P/P2CP/'
#                               Activate compiler-specific code
COMINC= -e 's/(\*[A-CE-Z]*'$(INC)'[A-CE-Z]*//g' \
         -e 's/[A-CE-Z]*'$(INC)'[A-CE-Z]*\*)//g'
#                               Change arc symbols to actual locations
SED0= -e 's/@/^/g' \
   -e "s/|startangle|/endpos.xpos/g" -e "s/|arcangle|/endpos.ypos/g"
#                               Fix the includes and version date
SED1=-e 's/^ *\(.include\) *[^ ]\([^ ]*\)[^ ] */\1 \"\2\"/' \
   -e "s/VERSIONDATE/dpic version $(DATE)/"
#                               Braces in C code
SEDC=-e 's/|\*/{/g' -e 's/\*|/}/g'

SED= sed $(COMPTABLES) $(DEBUG) $(COMINC) $(SED0) $(SED1)

# --------------------------------------------------------------------- #

.SUFFIXES:	.h .x .p
.x.h:
	cat $*.x | $(SED) > $*.h
.x.p:
	cat $*.x | $(SED) > $*.p

# --------------------------------------------------------------------- #

LEXCINCLUDES = entryhp.h entrytv.h lxhp.h lxnp.h lxtv.h lxch.h

ccode: dpic.c $(LEXCINCLUDES) parstab.i

#                               C includes (short int for static tables)
dpic.c: dpic.b
	sed \
    -e 's/int lr\[.*$$/short lr[lrmax + 1]={\
#include "parstab.i" \
      };/' \
    -e '/lxinx entryhp\[/s/;/={\
#include "entryhp.h"\
      };/' \
    -e '/lxinx lxhp\[/s/;/={\
#include "lxhp.h"\
      };/' \
    -e '/lxinx lxnp\[/s/;/={\
#include "lxnp.h"\
      };/' \
    -e '/symbol lxtv\[/s/;/={\
#include "lxtv.h"\
      };/' \
    -e '/symbol entrytv\[/s/;/={\
#include "entrytv.h"\
      };/' \
    -e '/Char lxch\[/s/;/={\
#include "lxch.h"\
      };/' \
	dpic.b > dpic.c

#                               C code tweaks and readable struct names
dpic.b: dpic.a U1.sed U2.sed
	sed -f sysdep.sed \
        -e 's/^main/void main/' \
        -e '1,4d' -e 's/UEMPTY6/Ublock/g' -e 's/UU/Upr/g' -e 's/WITH/With/g' \
        -e 's/Static //' \
        -e '/random()  *\//s/SHORT_MAX/LONG_MAX/' \
        -e '/define.*randmax/s/SHORT_MAX/(double) LONG_MAX/' \
        -e 's/PASCAL_MAIN(.*$$/P_argc = argc; P_argv = argv; __top_jb = NULL;/'\
        -e 's%exit(EXIT_SUCCESS);%if (errcount==0) exit(EXIT_SUCCESS); \
         else exit(EXIT_FAILURE);%' \
    dpic.a | sed -f U1.sed | sed -f U2.sed > dpic.b

#                               Convert pascal to C
dpic.a: dpic.p $(P2C)
	$(P2C) -M0 -o dpic.a dpic.p

CTABLES = parstab.i lextable.i
PARSCONSTS = parscst.i parscdc.i
LEXERRORS = parserr.i
LEXEDIT = lexedit1 lexedit2

#                               Combine pascal files for p2c conversion
dpic.p: $(PARSCONSTS) $(LEXERRORS) $(LEXEDIT) controlerr.i sysdep.h \
      mfp.h mpo.h pdf.h pgf.h ps.h pst.h svg.h tex.h xfig.h \
      dpic.x parserr.i lxcst.h dp0.h dpic1.p
	echo "{Integer16=2}" > dpic.p
	cat dpic.x | $(SED) $(SEDC) | $(CPP) >> dpic.p

$(P2C):
	if test $(P2C) != p2c ; then (cd p2c; mkdir -p home; cd src ; \
     dir=`pwd`; \
     make HOMEDIR=$$dir/../home INCDIR=$$dir/../home/p2c BINDIR=$$dir/.. \
          LIBDIR=$$dir/../home MANDIR=$$dir/../home MANFILE=p2c.cat \
          DATE=$(DATE) CC=$(CC) install ; \
     mv p2c .. ; make clean) ; fi

#                               sed code for readable struct names
U1.sed U2.sed: lxcst.h
	cat lxcst.h | sed -e '/(\*/d' | tr ';' '\012' \
     | sed -e 's/ XL//' -e 's/ X//' \
        -e 's%^\([^ ][^ ]*\) = \([0-9][0-9]*\)%\2 s/U\2/U\1/g%' \
     | sort -rn | sed -e 's/^[0-9][0-9]* //' | sed -e '/^ *$$/d' > U.sed
	sed -n '1,99p' U.sed > U1.sed
	sed -n '100,$$p' U.sed > U2.sed
	rm -f U.sed

CMPRS = awk '{ b=b" "$$0; if(length(b) > 60){print b; b=""}}; END{print b}'
#                               Change lexical symbols to values
lxcst.h: lxcst.v lexedit1 lexedit2
	sed -e '/^(\*/d' lxcst.v \
     | $(SED) | sed -f lexedit1 | sed -f lexedit2 | $(CMPRS) > lxcst.h

#                               Readable lexical error messages
controlerr.i: lexedit1 lexedit2 parserr.i
	sed -e "s%^%/%" -e "s% *$$%/d%" lexremoves > removedata
	cat lexedit1 lexedit2 | sed -e "/METAGOAL/d" -e "/<EOF>/d" -f removedata \
        -e "s%^s/\(.*\)/\([0-9][0-9]*\)/g$$%\2: write(errout,'\1');%" \
        -e "s/'</'/" -e "s/>'/'/" -e "s%'\\\\%'%" -e "s%'''%''''%" \
     | sort -n \
     | awk '$$1==last {printf(" %s",$$0); next}; \
        {printf("\n%s",$$0); last=$$1}; END {printf("\n")}' \
     | sed -e "s%'); [0-9][0-9]*: write(errout,'% or %g" \
     | sed -e "s%')'%'Possibly unbalanced parentheses: )'%" \
     | sed -e "s%']'%'Possibly unbalanced brackets: ]'%" \
     | sed -e "s%'}'%'Possibly unbalanced braces: }'%" \
     | sed -e "s%';'%'; or end of line'%" \
     | sed -e "s%'\"'%'\" or string'%" \
     | sed -e "s%'\\$$'%'\\$$ or argument reference'%" \
    > controlerr.i

$(LEXCINCLUDES): lextable.i
	awk -f mkCtables.awk lextable.i
	chmod 644 *.i *.h

$(CTABLES) $(PARSCONSTS) $(LEXERRORS) $(LEXEDIT): dpic.bnf \
      lexremoves nullprods lxcst.v
	(cd tables; make CC=$(CC) )
	for name in $(CTABLES) $(PARSCONSTS) $(LEXERRORS) $(LEXEDIT) ; do \
      cp tables/$$name . ; done;

clean:
	rm -f dpic *.o dpic.c *~ log dpic.p dpic.b dpic.a
	rm -f *.h *.p *.i removedata zzz
	rm -f U.sed U1.sed U2.sed lexedit* lextable* parst*
	rm -rf tmp
	(cd p2c/src && make -f Makefile.default clean )
	(cd Pgen/src && make -f Makefile.default clean )
	(cd pascal && make -f Makefile.default clean )

veryclean: clean
	rm -f *.h *.i *.p
	(cd tables && make veryclean)
	(cd p2c && rm -rf home p2c* )
	(cd Pgen && rm -f Pgen* PG* pg* )
