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

# lab01
set(EXEC Coen371_Project)

file(GLOB SRC src/*.cpp)
file(GLOB EVAN_SRC src/Evan-models/*.cpp)
file(GLOB MATT_SRC src/matt-models/*.cpp)
file(GLOB SABRINA_SRC src/Sabrina-models/*.cpp)


add_executable(${EXEC} ${SRC} ${EVAN_SRC} ${MATT_SRC} ${SABRINA_SRC})

target_link_libraries(${EXEC} OpenGL::GL glew_s glfw glm)

list(APPEND BIN ${EXEC})
# end lab01

# install files to install location
install(TARGETS ${BIN} DESTINATION ${CMAKE_INSTALL_PREFIX})

