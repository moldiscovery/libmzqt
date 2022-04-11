#Cmake start parameters
cmake_minimum_required(VERSION 3.16)
project(mzqt)

set(CMAKE_AUTOUIC ON)

#Find packages that will be used
find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS AxContainer REQUIRED)

add_library(mzqt
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

if(mzqt)
    target_compile_definitions(mzqt PUBLIC
        MZQT_XRAWFILE_WRAPPER
    )
endif()
