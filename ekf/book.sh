for i in *md ; do pandoc $i -H header.tex -o ${i}.pdf --pdf-engine=xelatex  ; done
pdftk optimal/intro.pdf Capitulo_01_Teoria_de_Sistemas_Lineales.md.pdf Capitulo_02_Teoria_de_la_Probabilidad.md.pdf capitulo_3.md.pdf Capitulo_04_Propagacion_de_Estados_y_Covarianzas.md.pdf capitulo_5.md.pdf Capitulo_13_Filtrado_de_Kalman_No_Lineal.md.pdf capitulo_13_2.md.pdf  cat output optimal/optimal_book.pdf
