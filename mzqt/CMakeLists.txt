#Cmake start parameters
cmake_minimum_required(VERSION 3.16)
project(mzqt)

set(CMAKE_AUTOUIC ON)

#Find packages that will be used
find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS AxContainer REQUIRED)

add_library(mzqt
    common/Debug.cpp common/Debug.h
    common/Exception.cpp common/Exception.h
    common/IDispatch.cpp common/IDispatch.h
    common/InstrumentInfo.h
    common/InstrumentInterface.cpp common/InstrumentInterface.h
    common/cominterface.cpp common/cominterface.h
    common/MSTypes.cpp common/MSTypes.h
    common/MSUtilities.cpp common/MSUtilities.h
    common/Scan.cpp common/Scan.h
    common/UVScan.h
    common/UVSpectrum.cpp common/UVSpectrum.h
    common/UVSpoint.h
    common/UVTypes.h
    converters/ReAdW/FilterLine.cpp converters/ReAdW/FilterLine.h
    converters/ReAdW/ThermoInterface.cpp
    converters/ReAdW/ThermoInterface.h
    converters/ReAdW/XRawfile.cpp converters/ReAdW/XRawfile.h
    converters/ReAdW/xrawfilewrapper.cpp converters/ReAdW/xrawfilewrapper.h
    converters/massWolf/DACExScanStats.cpp converters/massWolf/DACExScanStats.h
    converters/massWolf/DACFunctionInfo.cpp converters/massWolf/DACFunctionInfo.h
    converters/massWolf/DACHeader.cpp converters/massWolf/DACHeader.h
    converters/massWolf/DACProcessInfo.cpp converters/massWolf/DACProcessInfo.h
    converters/massWolf/DACScanStats.cpp converters/massWolf/DACScanStats.h
    converters/massWolf/DACSpectrum.cpp converters/massWolf/DACSpectrum.h
    converters/massWolf/MassLynxInterface.cpp converters/massWolf/MassLynxInterface.h
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
