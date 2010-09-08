ASSIGN := iros2011
.PHONY : clean view fullclean check

run : ${ASSIGN}.latex 
	latex ${ASSIGN}.latex

full: run
	bibtex ${ASSIGN}
	latex ${ASSIGN}.latex
	latex ${ASSIGN}.latex
	dvips -t letter ${ASSIGN}.dvi
	ps2pdf13 ${ASSIGN}.ps

bib :
	wget http://www.citeulike.org/bibtex/user/bobsherbert/tag/iros2011
	mv iros2011 sources.bib

pdf : 
	dvips -t letter ${ASSIGN}.dvi
	ps2pdf13 ${ASSIGN}.ps

clean : 
	-rm *.aux
	-rm ${ASSIGN}.dvi
	-rm ${ASSIGN}.log
	-rm ${ASSIGN}.ps
	-rm ${ASSIGN}.toc
	-rm ${ASSIGN}.blg
	-rm ${ASSIGN}.bbl

view :
	evince ${ASSIGN}.pdf

fullclean : clean
	-rm ${ASSIGN}.pdf

check : 
	aspell -c --add-filter tex ${ASSIGN}.latex
	
