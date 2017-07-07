#!/bin/sh
gmtset BASEMAP_TYPE plain HEADER_FONT_SIZE 22p PAPER_MEDIA a4 X_AXIS_LENGTH 25c
psbasemap `minmax -I0.001 tmp.dft` -B:."Amplitude Specter":WeSn -JXh -K  > spec.ps
psxy tmp.dft -R -JXh -W1p,0/0/0 -O  >> spec.ps
psbasemap `minmax -I0.001 func.dat` -B:."Function":WeSn -JXh -K  > func.ps
psxy func.dat -R -JXh -W1p,0/0/0 -O  >> func.ps
gv spec.ps &
gv func.ps &
