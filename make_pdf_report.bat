@echo off
cd out
pdflatex report.tex
del report.aux
del report.log