# Course:   S�ov� aplikace a spr�va s�t�
# Project:  Projekt - Sniffer CDP a LLDP
# Author:   Radim Loskot, xlosko01@stud.fit.vutbr.cz
# Date:     11. 11. 2011
# 

OS=$(uname)

if [ $OS = "FreeBSD" ]; then
    gmake -f Makefile.am $*
else
    make -f Makefile.am $*
fi