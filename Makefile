# Course:   SÌùovÈ aplikace a spr·va sÌtÌ
# Project:  Projekt - Sniffer CDP a LLDP
# Author:   Radim Loskot, xlosko01@stud.fit.vutbr.cz
# Date:     11. 11. 2011
#
# Universal Makefile which calls script run_make.sh 
#
# Usage:
#	- make            compile project - release version
#	- make pack       packs all required files to compile this project    
#	- make clean      clean temp compilers files    
#	- make clean-all  clean all compilers files - includes project    
#	- make clean-outp clean output project files 
#

MK_SCRIPT=run_make.sh

all:
	chmod +x $(MK_SCRIPT)
	./$(MK_SCRIPT)


.PHONY: clean clean-all clean-outp pack test debug release run

pack:
	./$(MK_SCRIPT) pack

clean:
	./$(MK_SCRIPT) clean

clean-outp:
	./$(MK_SCRIPT) clean-outp

clean-all: clean clean-outp
	./$(MK_SCRIPT) clean-all

debug:
	./$(MK_SCRIPT) -B all CXXOPT=-g3
	
release:
	./$(MK_SCRIPT) -B all CXXOPT=-O3

run:
	./$(MK_SCRIPT) run