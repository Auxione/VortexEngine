cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(dependencies)

project(VortexEngine)

set(ENV{VULKAN_SDK} "C:\\VulkanSDK\\1.3.204.1")
find_package(Vulkan REQUIRED)

#sources
set(VORTEX_ENGINE_SOURCES
        #audio
        src/Vortex/Audio/AudioAPI.cpp
        src/Vortex/Audio/OpenALBackend.cpp

        #common
        src/Vortex/Common/Console.cpp
        src/Vortex/Common/DynamicLibrary.cpp
        src/Vortex/Common/ThreadPool.cpp

        #core
        src/Vortex/Core/Application.cpp
        src/Vortex/Core/Input.cpp

        #debug
        src/Vortex/Debug/Profiler.cpp

        #graphics
        src/Vortex/Graphics/GraphicsAPI.cpp
        src/Vortex/Graphics/ImageLoader.cpp
        src/Vortex/Graphics/LineRenderer.cpp

        #memory
        )

#Platform: OpenGL45 sources
set(VORTEX_ENGINE_SOURCES_OGL45
        src/Vortex/Platform/OpenGL45/OpenGL45Renderer.cpp
        )

if (CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    #Build as executable
    add_executable(
            VortexEngine
            src/Vortex/Core/EntryPoint.cpp
            ${VORTEX_ENGINE_SOURCES}
            ${VORTEX_ENGINE_SOURCES_OGL45}
    )
    set_target_properties(
            VortexEngine
            PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
    message(STATUS "[Vortex] Building as executable.")
else ()
    #Build as static library
    add_library(
            VortexEngine
            STATIC
            ${VORTEX_ENGINE_SOURCES}
            ${VORTEX_ENGINE_SOURCES_OGL45}
    )

    install(TARGETS VortexEngine
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

    message(STATUS "[Vortex] Building as static library.")
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(VortexEngine PUBLIC VORTEX_DEBUG) # VORTEX_DEBUG macro
endif ()

target_precompile_headers(VortexEngine
        PUBLIC src/Vortex/pch.h
        )

target_include_directories(VortexEngine
        #main
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/

        #backends & platforms
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/Vortex/Platform/

        #dependencies
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/glfw/include/
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/glad33/include/
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/stb/include/
        )

target_link_Libraries(VortexEngine
        PRIVATE glfw
        PRIVATE glad33
        PRIVATE stb
        PRIVATE OpenAL
        PRIVATE Vulkan::Vulkan
        )

#auto-ignore build directory
if (NOT EXISTS ${PROJECT_BINARY_DIR}/.gitignore)
    file(WRITE ${PROJECT_BINARY_DIR}/.gitignore "*")
endif ()
