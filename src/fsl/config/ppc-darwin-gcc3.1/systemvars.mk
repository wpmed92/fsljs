 


# for SHELL, do not change the type of shell - only use Bourne or BASH
SHELL = /bin/sh


# Compiler dependent variables

CC = cc
CXX = c++
CSTATICFLAGS = 
CXXSTATICFLAGS = 


ARCHFLAGS = 

DEPENDFLAGS = -MM

OPTFLAGS =  -O3 -fexpensive-optimizations ${ARCHFLAGS}
MACHDBGFLAGS = -g
GNU_ANSI_FLAGS = -traditional-cpp -Wall -Wno-long-long -Wno-long-double -ansi -pedantic
SGI_ANSI_FLAGS = -ansi -fullwarn
ANSI_FLAGS = ${GNU_ANSI_FLAGS}
 
 
# Variables determined with AUTOCONFIG: 
 
INSTALL = install -p -c
RM = /bin/rm
CP = /bin/cp
CHMOD = /bin/chmod
MKDIR = /bin/mkdir
RANLIB = ranlib
TCLSH = ${FSLDIR}/bin/tclsh
