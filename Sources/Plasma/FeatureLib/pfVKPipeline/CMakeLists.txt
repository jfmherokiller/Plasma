set(pfVKPipeline_SOURCES
    plVKDevice.cpp
    plVKEnumerate.cpp
    plVKDeviceRefs.cpp
    plVKPipeline.cpp
    plVKPlateManager.cpp
)

set(pfVKPipeline_HEADERS
    plVKDevice.h
    plVKDeviceRef.h
    plVKPipeline.h
    pfVKPipelineCreatable.h
    plVKPlateManager.h
)

plasma_library(pfVKPipeline SOURCES ${pfVKPipeline_SOURCES} ${pfVKPipeline_HEADERS}
        PRECOMPILED_HEADERS Pch.h
)
target_link_libraries(pfVKPipeline
    PUBLIC
        CoreLib
        pnNucleusInc
        plPipeline
    PRIVATE
        plStatusLog
        epoxy::epoxy
        Vulkan::Vulkan
    INTERFACE
        pnFactory
)
target_include_directories(pfVKPipeline
    PRIVATE
         ${VULKAN_INCLUDE_DIRS}
)

source_group("Source Files" FILES ${pfVKPipeline_SOURCES})
source_group("Header Files" FILES ${pfVKPipeline_HEADERS})
