# File:      Makefile
# Date:      May 2013
# Course:    PDS - Advanced Operating Systems
# Project:   Ticket algorithm
# Author:    Radim Loskot - xlosko01
# Compiler:  implicit (g++)

# Pouziti:
#  - make               compile release version
#  - make run           run program
#  - make pack          packs all required files to compile this project    
#  - make clean         clean temp compilers files    

# output project and package filename
SRC_DIR=src
OBJ_DIR=obj
TARGET=lpm
PACKAGE_NAME=xlosko01
PACKAGE_FILES=dokumentace.pdf Makefile Makefile.am run_make.sh src/longest_prefix.cpp src/AddrFamilies.h src/AddrTrie.h src/AddrTrieBase.cpp src/AddrTrieBase.h src/TrieNode.h

# C++ compiler and flags
CXX=g++
CXXFLAGS=$(CXXOPT) -Wall -pedantic -W -ansi -O3
LIBS=-lpthread #-lpthreads

# Project files
OBJ_FILES=longest_prefix.o AddrTrieBase.o
SRC_FILES=longest_prefix.cpp AddrTrieBase.cpp

# Substitute the path
SRC=$(patsubst %,$(SRC_DIR)/%,$(SRC_FILES))

OBJ=$(patsubst %,$(OBJ_DIR)/%,$(OBJ_FILES))

# Universal rule
$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

# START RULE
all: | $(OBJ_DIR) $(TARGET)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Linking of modules into release program
$(TARGET): $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

.PHONY: clean pack run debug release

pack:
	zip $(PACKAGE_NAME).zip $(PACKAGE_FILES)

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(TARGET)

debug:
	make -B all CXXOPT=-g3
	
release:
	make -B all CXXOPT=-O3

run:
	./$(TARGET) -i asns.txt <ip.txt