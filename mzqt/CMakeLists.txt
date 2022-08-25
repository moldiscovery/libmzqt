cmake_minimum_required(VERSION 3.18)
project(mzqt)

option(MZQT_USE_MEMORY_CHECK_MMGR      "" OFF)
option(MZQT_USE_XRAWFILE_WRAPPER       "" OFF)
option(MZQT_USE_STATIC                 "" OFF)

set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS AxContainer REQUIRED)

add_library(mzqt SHARED
    common/Debug.cpp
    common/Exception.cpp
    common/IDispatch.cpp
    common/InstrumentInfo.h
    common/InstrumentInterface.cpp
    common/cominterface.cpp
    common/MSTypes.cpp
    common/MSUtilities.cpp
    common/Scan.cpp
    common/UVScan.h
    common/UVSpectrum.cpp
    common/UVSpoint.h
    common/UVTypes.h
    converters/ReAdW/FilterLine.cpp
    converters/ReAdW/ThermoInterface.cpp
    converters/ReAdW/XRawfile.cpp
    converters/ReAdW/xrawfilewrapper.cpp
    converters/massWolf/DACExScanStats.cpp
    converters/massWolf/DACFunctionInfo.cpp
    converters/massWolf/DACHeader.cpp
    converters/massWolf/DACProcessInfo.cpp
    converters/massWolf/DACScanStats.cpp
    converters/massWolf/DACSpectrum.cpp
    converters/massWolf/MassLynxInterface.cpp
)

target_include_directories(mzqt PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/converters/ReAdW
    ${CMAKE_CURRENT_SOURCE_DIR}/converters/massWolf
    ${CMAKE_CURRENT_SOURCE_DIR}/common
)

target_compile_definitions(mzqt PUBLIC
    MZQTDLL_EXPORTS
)

target_link_libraries(mzqt PUBLIC
    Qt5::Core
    Qt5::AxContainer
)

if(WIN32 AND MSVC)
    target_compile_definitions(mzqt PUBLIC
        MZQTDLL_EXPORTS
    )
endif()

if(MZQT_USE_MEMORY_CHECK_MMGR)
    target_sources(mzqt PRIVATE
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

if(MZQT_USE_STATIC)
    target_compile_definitions(mzqt PUBLIC
        MZQT_STATIC
    )
endif()

if(MZQT_USE_XRAWFILE_WRAPPER)
    target_compile_definitions(mzqt PUBLIC
        MZQT_XRAWFILE_WRAPPER
    )
endif()
