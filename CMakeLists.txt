cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(dependencies)

project(VortexEngine)

#sources
set(VORTEX_ENGINE_SOURCES
        #audio
        src/Vortex/Audio/AudioAPI.cpp
        src/Vortex/Audio/OpenALBackend.cpp

        #common
        src/Vortex/Common/ThreadPool.cpp

        #core
        src/Vortex/Core/Application.cpp
        src/Vortex/Core/Input.cpp

        #debug
        src/Vortex/Debug/Profiler.cpp

        #graphics
        src/Vortex/Graphics/GLFWBackend.cpp
        src/Vortex/Graphics/GraphicsAPI.cpp
        src/Vortex/Graphics/ImageLoader.cpp
        src/Vortex/Graphics/OpenGL45Backend.cpp
        src/Vortex/Graphics/Renderer.cpp

        #math
        src/Vortex/Math/Quaternion.cpp
        src/Vortex/Math/Random.cpp
        src/Vortex/Math/VortexMath.cpp

        #memory
        src/Vortex/Memory/HeapAllocator.cpp

        )

#public headers
set(VORTEX_ENGINE_PHEADERS
        #audio
        include/Vortex/Audio/AudioAPI.h
        include/Vortex/Audio/AudioBackend.h

        #common
        include/Vortex/Common/FNV1Hasher.h
        include/Vortex/Common/Handle.h
        include/Vortex/Common/HashedString.h
        include/Vortex/Common/Logger.h
        include/Vortex/Common/Signal.h
        include/Vortex/Common/Singleton.h
        include/Vortex/Common/ThreadPool.h
        include/Vortex/Common/Timer.h

        #containers
        include/Vortex/Containers/List.h
        include/Vortex/Containers/Queue.h
        include/Vortex/Containers/RawBuffer.h
        include/Vortex/Containers/Vector.h

        #core
        include/Vortex/Core/Application.h
        include/Vortex/Core/Event.h
        include/Vortex/Core/Input.h
        include/Vortex/Core/Keycode.h

        #debug
        include/Vortex/Debug/Assert.h
        include/Vortex/Debug/Profiler.h

        #graphics
        include/Vortex/Graphics/GraphicsAPI.h
        include/Vortex/Graphics/ImageLoader.h
        include/Vortex/Graphics/RenderBackend.h
        include/Vortex/Graphics/Renderer.h
        include/Vortex/Graphics/WindowBackend.h

        #math
        include/Vortex/Math/BasicAngle.h
        include/Vortex/Math/BasicCircle.h
        include/Vortex/Math/BasicGUID.h
        include/Vortex/Math/BasicLineSegment2.h
        include/Vortex/Math/BasicMatrix.h
        include/Vortex/Math/BasicRectangle.h
        include/Vortex/Math/BasicTriangle.h
        include/Vortex/Math/BasicVector.h
        include/Vortex/Math/MathToString.h
        include/Vortex/Math/Random.h
        include/Vortex/Math/VortexMath.h
        include/Vortex/Math/VortexMathConstants.h

        #memory
        include/Vortex/Memory/Allocator.h
        include/Vortex/Memory/HeapAllocator.h
        include/Vortex/Memory/Memory.h
        include/Vortex/Memory/SmartPointers.h

        )

#Build as static library
add_library(
        VortexEngine
        STATIC
        ${VORTEX_ENGINE_SOURCES}
)
set_target_properties(VortexEngine PROPERTIES PUBLIC_HEADER
        ${VORTEX_ENGINE_PHEADERS}
        )

install(TARGETS VortexEngine
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})


if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(VortexEngine PUBLIC VORTEX_DEBUG)
endif ()

target_precompile_headers(VortexEngine
        PUBLIC src/Vortex/pch.h
        )

target_include_directories(VortexEngine
        #main
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/

        #backends
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/

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
        )

#auto-ignore build directory
if (NOT EXISTS ${PROJECT_BINARY_DIR}/.gitignore)
    file(WRITE ${PROJECT_BINARY_DIR}/.gitignore "*")
endif ()