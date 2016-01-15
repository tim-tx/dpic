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

Dpicdoc.pdf: dpicdoc.aux
	pdflatex dpicdoc
	pdflatex dpicdoc

dpicdoc.aux: dpicdoc.tex dpicdoc.bib $(PICS) dpic_man.tex
	pdflatex dpicdoc
	bibtex dpicdoc

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

clobber: clean
	rm -f *.pdf dpic_man.tex

clean:
	rm -f *.ps *.dvi *.aux *.log *.blg *.ilg *.toc core *.bbl *.pdf *.upa *.upb
	rm -f $(PICS) Spiral.pic dpictools.pic
