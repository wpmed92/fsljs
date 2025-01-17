# $Id: externallibs.mk,v 1.1 2004/07/09 13:06:21 duncan Exp $

# External Library and Include Paths

FSLEXTLIB=${FSLDIR}/extras/lib
FSLEXTINC=${FSLDIR}/extras/include

# CEPHES library
LIB_CEPHES = ${FSLEXTLIB}
INC_CEPHES = ${FSLEXTINC}/cephes

# FFTW library
LIB_FFTW = ${FSLEXTLIB}
INC_FFTW = ${FSLEXTINC}/fftw

# GD library
LIB_GD = ${FSLEXTLIB}
INC_GD = ${FSLEXTINC}/libgd

# GDC library
LIB_GDC = ${FSLEXTLIB}
INC_GDC = ${FSLEXTINC}/libgdc

# GSL library
LIB_GSL = ${FSLEXTLIB}
INC_GSL = ${FSLEXTINC}/gsl

# PNG library
LIB_PNG = ${FSLEXTLIB}
INC_PNG = ${FSLEXTINC}/libpng

# PROB library
LIB_PROB = ${FSLEXTLIB}
INC_PROB = ${FSLEXTINC}/libprob

# CPROB library
LIB_CPROB = ${FSLEXTLIB}
INC_CPROB = ${FSLEXTINC}/libcprob

# NEWMAT library
LIB_NEWMAT = ${FSLEXTLIB}
INC_NEWMAT = ${FSLEXTINC}/newmat

# NEWRAN library
LIB_NEWRAN = ${FSLEXTLIB}
INC_NEWRAN = ${FSLEXTINC}/newran

# ZLIB library
LIB_ZLIB = ${FSLEXTLIB}
INC_ZLIB = ${FSLEXTINC}/zlib

# BOOST library
BOOSTDIR = /usr/local/boost
LIB_BOOST = ${BOOSTDIR}
INC_BOOST = ${BOOSTDIR}

# QT library
QTDIR = /usr/lib/qt3
LIB_QT = ${QTDIR}/lib
INC_QT = ${QTDIR}/include

# QWT library
QWTDIR = /usr/local/qwt
LIB_QWT = ${QWTDIR}/lib
INC_QWT = ${QWTDIR}/include

