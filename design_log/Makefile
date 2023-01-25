default: clearpaper paper.pdf clean

clearpaper:
	rm -f paper.pdf

paper.pdf: paper.tex structure.tex sources.bib # *.png
	pdflatex paper.tex
#	bibtext sources.bib
# 	pdflatex paper.tex

clean:
	rm -f *.log *.aux *.bbl *.blg *.out *.bcf *.xml