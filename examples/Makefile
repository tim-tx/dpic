DPIC = dpic
# DPIC = $$HOME/dpic/test/dpic

nooption:
	cp sources/diag1.pic diag.pic
	cp sources/test1.tex tst.tex
	$(DPIC) diag.pic > diag.tex
#	latex tst
#	dvips -Ppdf tst.dvi

all: nooption epic pstricks pgf pdf mfpic psfrag postscript overlay metapost \
  xfig svg

epic:
	cp sources/diag2.pic diag.pic
	cp sources/test2.tex tst.tex
	$(DPIC) -t diag.pic > diag.tex
	latex tst
	dvips -Ppdf tst.dvi

pstricks:
	cp sources/diag1.pic diag.pic
	cp sources/test3.tex tst.tex
	$(DPIC) -p diag.pic > diag.tex
	latex tst
	dvips -Ppdf tst.dvi

pgf:
	cp sources/diag1.pic diag.pic
	cp sources/test9.tex tst.tex
	$(DPIC) -g diag.pic > diag.tex
	latex tst
	dvips -Ppdf tst.dvi

pdf:
	cp sources/diag1.pic diag.pic
	cp sources/test9.tex tst.tex
	$(DPIC) -g diag.pic > diag.tex
	pdflatex tst

mfpic:
	cp sources/diag1.pic diag.pic
	cp sources/test4.tex tst.tex
	$(DPIC) -m diag.pic > diag.tex
	latex tst
	mf tst-mfpic
	latex tst
	dvips tst.dvi

psfrag:
	cp sources/diag1.pic diag.pic
	cp sources/test5.tex tst.tex
	$(DPIC) -f diag.pic > diag.eps
	latex tst
	dvips -Ppdf tst

postscript:
	cp sources/diag6.pic diag.pic
	cp sources/test6.tex tst.tex
	$(DPIC) -r diag.pic > diag.eps
	latex tst
	dvips -Ppdf tst

svg:
	cp sources/diag6.pic diag.pic
	cp sources/test6.tex tst.tex
	$(DPIC) -v diag.pic > tst.svg

overlay:
	$(DPIC) -r sources/diagA.pic > diag.eps
	$(DPIC) -p sources/diagB.pic > diag.tex
	cp sources/test7.tex tst.tex
	latex tst
	dvips -Ppdf tst.dvi

metapost:
	cp sources/diag8.pic diag.pic
	cp sources/test8.tex tst.tex
	$(DPIC) -s diag.pic > diag.mp
	mpost --tex=latex diag.mp
	latex tst
	dvips -Ppdf tst

xfig:
	cp sources/diag9.pic diag.pic
	$(DPIC) -x diag.pic > diag.fig

clean:
	rm -f *.ps *.eps *.dvi *.log *.mp *.1 mpxerr* *pk *gf *.mpx *.dim *.mf
	rm -f *.tfm *.aux *.pfg tst.tex diag.pic diag.tex *.trace *.fig *.pdf *.svg
