# Build GLEW
include(FetchContent)

mark_as_advanced(FETCHCONTENT_QUIET)
mark_as_advanced(FETCHCONTENT_FULLY_DISCONNECTED)
mark_as_advanced(FETCHCONTENT_UPDATES_DISCONNECTED)

set(FETCHCONTENT_UPDATES_DISCONNECTED ON) # speed up config

set(THIRDPARTY thirdparty-assimp)

FetchContent_Declare(${THIRDPARTY}
    URL ${CMAKE_SOURCE_DIR}/thirdparty/assimp-master.zip
)

mark_as_advanced(FETCHCONTENT_BASE_DIR)

FetchContent_GetProperties(${THIRDPARTY})

string(TOUPPER ${THIRDPARTY} UTHIRDPARTY)

mark_as_advanced(FETCHCONTENT_SOURCE_DIR_${UTHIRDPARTY})
mark_as_advanced(FETCHCONTENT_UPDATES_DISCONNECTED_${UTHIRDPARTY})

if(NOT ${THIRDPARTY}_POPULATED)
    FetchContent_Populate(${THIRDPARTY})

    add_subdirectory(
        ${${THIRDPARTY}_SOURCE_DIR}
        ${${THIRDPARTY}_BINARY_DIR}
        EXCLUDE_FROM_ALL
    )
    target_include_directories(assimp INTERFACE $<BUILD_INTERFACE:${${THIRDPARTY}_SOURCE_DIR}/include>)
endif()

# hide GLEW options
mark_as_advanced(BUILD_UTILS)
mark_as_advanced(BUILD_SHARED_LIBS)
mark_as_advanced(BUILD_FRAMEWORK)

set(BUILD_UTILS OFF)
set(BUILD_SHARED_LIBS OFF)
