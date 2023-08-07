cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_STANDARD 17)

project(Coen371_Project)

include(ExternalProject)

# Set install directory
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/dist CACHE PATH ${CMAKE_SOURCE_DIR}/dist FORCE)
if(WIN32)
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Debug;Release" FORCE)
endif()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(OpenGL REQUIRED COMPONENTS OpenGL)

include(BuildGLEW)
include(BuildGLFW)
include(BuildGLM)
include_directories("thirdparty/stb")
include_directories("thirdparty/irrKlang/include")

# lab01
set(EXEC Coen371_Project)

file(GLOB SRC src/*.cpp)
file(GLOB EVAN_SRC src/Evan-models/*.cpp)
file(GLOB SABRINA_SRC src/Sabrina-models/*.cpp)

add_executable(${EXEC} ${SRC} ${EVAN_SRC} ${SABRINA_SRC})
link_libraries(-L/thirdparty/irrKlang/lib/irrKlang.dll)
target_link_libraries(${EXEC} OpenGL::GL glew_s glfw glm )

list(APPEND BIN ${EXEC})
# end lab01

# install files to install location
install(TARGETS ${BIN} DESTINATION ${CMAKE_INSTALL_PREFIX})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/irrKlang/lib/irrKlang.dll
     DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/out/build/x64-Debug)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/irrKlang/lib/irrKlang.dll
     DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Debug)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/irrKlang/lib/irrKlang.dll
        DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/dist)
