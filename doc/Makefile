DPIC = dpic

.SUFFIXES:	.pic .tex .m4 .pdf
.pic.tex:
	$(DPIC) -g $*.pic > $*.tex
.m4.tex:
	m4 pgf.m4 $*.m4 > $*.pic
	$(DPIC) -g $*.pic > $*.tex
.m4.pdf:
	m4 pdf.m4 $*.m4 > $*.pic
	$(DPIC) -d $*.pic > $*.pdf

PICS = spline.tex arrow.tex circles.tex Spiral.tex
PDFLATEX = pdflatex --quiet

Dpicdoc.pdf: dpic-doc.aux
	$(PDFLATEX) dpic-doc
	$(PDFLATEX) dpic-doc

dpic-doc.aux: dpic-doc.tex dpic-doc.bib $(PICS) dpic_man.tex VERSION.tex
	$(PDFLATEX) dpic-doc
	bibtex dpic-doc

VERSION.tex: VERSION
	cp VERSION VERSION.tex

dpic.man: dpic.1
	nroff -man dpic.1 | sed -e 's%.%%g' -e '/SunOS/d' > dpic.man

dpic_man.tex: dpic.1
	sed -e 's/\.SS/SSSS/' -e 's/\\(hy/-/g' -e '/^.tr/d' \
     -e 's/^.br/BBBB/' -e 's/\\(ts/QQQQ/g' -e 's/^\.P$$//' -e 's/\\-/-/g' \
     -e 's/\\e/TTTT/'g \
     -e 's/^\.\\" {\\\\tt/{\\\\tt/' \
     -e 's/^\.\\" }/}/' \
     dpic.1 > xx
	tr2latex -b -m < xx | sed -e '1,9d' \
     -e 's/\\bf/\\tt/g' \
     -e 's/SSSS \(.*\)$$/\\par\\hskip-2pc{\\bf \1}\\\\/' \
     -e 's/QQQQ/{\\tt "}/g' -e 's/BBBB/\\\\\\hbox{}\\hskip-1pt/' \
     -e 's/TTTT/{\\tt \\char92}/g' \
     -e 's/http:\([^ ]*\)/\\url{http:\1}/' -e 's/\\~{}/\~/' \
      > dpic_man.tex
	rm -f xx

dpic.1: dpic.1.src
	sed -e "/^\.TH/s/\".*$$/\"`cat VERSION`\"/" dpic.1.src > dpic.1

clean:
	rm -f *.ps *.dvi *.aux *.log *.blg *.ilg *.toc core *.bbl *.upa *.upb

clobber: clean
	rm -f *.pdf VERSION.tex $(PICS) Spiral.pic dpic_man.tex dpic.1
