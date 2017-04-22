# Generate tables from the bnf file

BNF = ../dpic.bnf
REMOVES = ../lexremoves
NULLPRODS = ../nullprods

PG= ../Pgen/PG
PGP= ../Pgen/PGp
PGC= ../Pgen/PGc
PGF= ../Pgen/PGf

# Uncomment if installed
#PG= PG
#PGP= PGp
#PGC= PGc
#PGF= PGf

CC=gcc
CTABLES = parstab.i lextable.i
TABLECODE = parscst.i parscdc.i parserr.i
LEXEDIT = lexedit1 lexedit2

$(CTABLES) $(TABLECODE) $(LEXEDIT): PG.out
#                               Tables: lextable, parstabl matrix
#                               Pascal: parscst, parscdc, parserr
#                               Lexical edit file: lexedit
	$(PGP) -f $(REMOVES) < PG.out
	mv parscdc parscdc.i
#                               parstab.i in linear C form
	$(PGC) -f $(NULLPRODS) < PG.out
	cp -p parstab parstab.i
	cp -p lextable lextable.i
#                               lrmax for linear table in parscst.i
	fgrep -o ',' parstab.i | wc -l > lrmax
	sed -e "/lrmax/s/[0-9][0-9]*/`cat lrmax`/" parscst > parscst.i
#                               Expected terminals: parserr.i
	awk -f ../mkerrs.awk PG.out | sed \
      -e 's/<END>/.PE/' -e 's/<START>/.PS/' -e 's/<endfor>/}/' \
      -e 's/ <ERROR>//' -e 's/rand <func1> <func2>/function/' \
      -e 's/<envvar> <name> <float>/constant variable/' \
      -e 's/<Label> <corner> Here last ` {/location/' \
      -e 's/last <float> ` {/enumerated obj/' \
      -e 's/last ` {/enumerated obj/' \
      -e 's/<name> <envvar>/variable/' \
      -e 's/<compare>/logic opr/' \
      -e 's/<\([a-zA-Z][a-zA-Z]*\)>/\1/g' \
      > parserr.i
#                               Spit lexedit for < 100 cmds per script file
	sed -n '1,99p' lexedit > lexedit1
	sed -n '100,$$p' lexedit > lexedit2
	chmod 644 lex* lr* pars*

#                               Raw lex and parse data
PG.out: $(BNF) $(PG) $(PGP) $(PGC)
	$(PG) $(BNF) > PG.out ; chmod 644 PG.out

$(PG) $(PGC) $(PGP):
	if test $(PG) != PG ; then (cd ../Pgen/src ; \
      make NAME=PG CC=$(CC) install ; make NAME=PG clean ) ; fi

clean:
	rm -f lrmax lex* pars* PG.list

veryclean: clean
