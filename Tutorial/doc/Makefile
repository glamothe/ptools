# 
# makefile for latex 
VERSION=20070206
# here is the latex filename without .tex
NAME = tutorial
#

pdf:	checkname $(NAME).tex	
		pdflatex $(NAME).tex
		pdflatex $(NAME).tex
		pdflatex $(NAME).tex

help:
	@echo "Makefile for latex compilation"
	@echo "edit this Makefile to add your file to compile" 
	@echo "version is $(VERSION)"

checkname:
ifdef NAME
	@echo "NAME is defined as: $(NAME)"
	@echo "version is $(VERSION)"
else
	@echo "ERROR !! NAME is not defined"
	@make --no-print-directory help
	@echo "abording..."
	@exit 1
endif

dvi:		checkname
			latex $(NAME).tex

ps:			dvi
			dvips $(NAME).dvi -o $(NAME).ps

pdffromps:	ps
			ps2pdf $(NAME).ps $(NAME).pdf


showpdf:	checkname $(NAME).pdf	
			acroread $(NAME).pdf &

showps:		checkname $(NAME).pdf
			gv $(NAME).ps &
	
bibtex:		checkname
			bibtex $(NAME)

clean:		checkname
			rm -f $(NAME).aux $(NAME).log $(NAME).dvi $(NAME).bbl $(NAME).blg $(NAME).tns $(NAME).vrb
			rm -f $(NAME).toc $(NAME).out $(NAME).snm $(NAME).nav

cleanalllog:	
			rm -f *.aux *.log *.dvi *.bbl *.blg *.tns *.vrb
			rm -f *.toc *.out *.snm *.nav

cleanall:	clean
			rm -f $(NAME).ps $(NAME).pdf

