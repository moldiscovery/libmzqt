#Cmake start parameters
cmake_minimum_required(VERSION 3.16)
project(mzqt)


#Cmake core for QT
#See: https://doc.qt.io/qt-6/cmake-get-started.html
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)


#Find packages that will be used
find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS AxContainer REQUIRED)

add_library(mzqt
    mzqt/common/Debug.cpp mzqt/common/Debug.h
    mzqt/common/Exception.cpp mzqt/common/Exception.h
    mzqt/common/IDispatch.cpp mzqt/common/IDispatch.h
    mzqt/common/InstrumentInfo.h
    mzqt/common/InstrumentInterface.cpp mzqt/common/InstrumentInterface.h
    mzqt/common/MSTypes.cpp mzqt/common/MSTypes.h
    mzqt/common/MSUtilities.cpp mzqt/common/MSUtilities.h
    mzqt/common/Scan.cpp mzqt/common/Scan.h
    mzqt/common/UVScan.h
    mzqt/common/UVSpectrum.cpp mzqt/common/UVSpectrum.h
    mzqt/common/UVSpoint.h
    mzqt/common/UVTypes.h
    mzqt/converters/ReAdW/FilterLine.cpp mzqt/converters/ReAdW/FilterLine.h
    mzqt/converters/ReAdW/ThermoInterface.cpp mzqt/converters/ReAdW/ThermoInterface.h
    mzqt/converters/ReAdW/XRawfile.cpp mzqt/converters/ReAdW/XRawfile.h
    mzqt/converters/massWolf/DACExScanStats.cpp mzqt/converters/massWolf/DACExScanStats.h
    mzqt/converters/massWolf/DACFunctionInfo.cpp mzqt/converters/massWolf/DACFunctionInfo.h
    mzqt/converters/massWolf/DACHeader.cpp mzqt/converters/massWolf/DACHeader.h
    mzqt/converters/massWolf/DACProcessInfo.cpp mzqt/converters/massWolf/DACProcessInfo.h
    mzqt/converters/massWolf/DACScanStats.cpp mzqt/converters/massWolf/DACScanStats.h
    mzqt/converters/massWolf/DACSpectrum.cpp mzqt/converters/massWolf/DACSpectrum.h
    mzqt/converters/massWolf/MassLynxInterface.cpp mzqt/converters/massWolf/MassLynxInterface.h
)

target_include_directories(mzqt PUBLIC
    mzqt
)

target_compile_definitions(mzqt PUBLIC
    MZQTDLL_EXPORTS
)

#Link the libraries that we have add with find_package
#This part must be done after add_library for this reason find_package and target_link_libraries are in different blocks
target_link_libraries(mzqt PUBLIC
    Qt5::Core
    Qt5::AxContainer
)

if(win32-msvc_x_)
    target_compile_definitions(mzqt PUBLIC
        MZQTDLL_EXPORTS
    )
endif()

if(memory_check_mmgr)
    target_sources(mzqt PUBLIC
        mmgr/mmgr.cpp mmgr/mmgr.h
        mmgr/nommgr.h
    )

    target_include_directories(mzqt PUBLIC
        mmgr
    )

    target_compile_definitions(mzqt PUBLIC
        USE_MMGR_MEMORY_CHECK
    )
endif()
