TEMPLATE = lib
DESTDIR = lib
MOC_DIR = tmp
OBJECTS_DIR = tmp
TARGET = mzqt
CONFIG += qaxcontainer
CONFIG += warn_on

QT -= gui

INCLUDEPATH = . \
    ./mzqt

xrawfile_wrapper {
    DEFINES += MZQT_XRAWFILE_WRAPPER
}

win32-msvc* {
    message(Compile msvc dynamic library)
    DEFINES += MZQTDLL_EXPORTS
}
else {
    message(Compile dynamic library)
}

mzqt_static {
  DEFINES += MZQT_STATIC
  CONFIG+=staticlib
}

HEADERS = mzqt/common/UVSpectrum.h \
    mzqt/common/UVTypes.h \
    mzqt/common/UVSpoint.h \
    mzqt/converters/ReAdW/XRawfile.h \
    mzqt/converters/ReAdW/xrawfilewrapper.h \
    mzqt/converters/massWolf/DACSpectrum.h \
    mzqt/converters/massWolf/DACExScanStats.h \
    mzqt/converters/massWolf/DACHeader.h \
    mzqt/converters/massWolf/DACScanStats.h \
    mzqt/converters/massWolf/DACFunctionInfo.h \
    mzqt/converters/ReAdW/FilterLine.h \
    mzqt/converters/ReAdW/ThermoInterface.h \
    mzqt/converters/massWolf/DACProcessInfo.h \
    mzqt/common/InstrumentInfo.h \
    mzqt/common/InstrumentInterface.h \
    mzqt/common/MSTypes.h \
    mzqt/common/MSUtilities.h \
    mzqt/common/Scan.h \
    mzqt/common/IDispatch.h \ 
    mzqt/common/Exception.h \
    mzqt/common/Debug.h \
    mzqt/converters/massWolf/MassLynxInterface.h \
    mzqt/common/UVScan.h \
    mzqt/common/cominterface.h

SOURCES = mzqt/common/UVSpectrum.cpp \
    mzqt/converters/ReAdW/XRawfile.cpp \
    mzqt/converters/ReAdW/xrawfilewrapper.cpp \
    mzqt/converters/massWolf/DACSpectrum.cpp \
    mzqt/converters/massWolf/DACHeader.cpp \
    mzqt/converters/massWolf/DACScanStats.cpp \
    mzqt/converters/massWolf/DACExScanStats.cpp \
    mzqt/converters/massWolf/DACFunctionInfo.cpp \
    mzqt/converters/ReAdW/FilterLine.cpp \
    mzqt/common/InstrumentInterface.cpp \
    mzqt/converters/ReAdW/ThermoInterface.cpp \
    mzqt/converters/massWolf/DACProcessInfo.cpp \
    mzqt/common/MSTypes.cpp \
    mzqt/common/MSUtilities.cpp \
    mzqt/common/Scan.cpp \
    mzqt/common/IDispatch.cpp \
    mzqt/common/Exception.cpp \
    mzqt/common/Debug.cpp \
    mzqt/converters/massWolf/MassLynxInterface.cpp \
    mzqt/common/cominterface.cpp

memory_check_mmgr {

    INCLUDEPATH += ./mmgr
    DEFINES += USE_MMGR_MEMORY_CHECK
	
    HEADERS += ./mmgr/mmgr.h \
	          ./mmgr/nommgr.h
	          
    SOURCES += ./mmgr/mmgr.cpp
}

