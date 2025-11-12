cmake_minimum_required(VERSION 3.16)

project(libcrobj
    VERSION 1.0.0
    DESCRIPTION "UI abstraction and rotation core for Cronos"
    LANGUAGES C
)

# ------------------------------------------------------------
# Build configuration
# ------------------------------------------------------------
option(ENABLE_LVGL_BACKEND "Enable LVGL backend support" ON)
option(BUILD_SHARED_LIBS "Build as shared library (.so)" ON)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Conservative warnings: Yocto may treat -Werror as fatal in SDK
add_compile_options(-Wall -Wextra)

# ------------------------------------------------------------
# Dependencies
# ------------------------------------------------------------
if(ENABLE_LVGL_BACKEND)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(LVGL REQUIRED lvgl)
    pkg_check_modules(LIBDRM REQUIRED libdrm)
endif()

# ------------------------------------------------------------
# Include directories
# ------------------------------------------------------------
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(ENABLE_LVGL_BACKEND)
    include_directories(${LVGL_INCLUDE_DIRS} ${LIBDRM_INCLUDE_DIRS})
endif()

# ------------------------------------------------------------
# Sources
# ------------------------------------------------------------
file(GLOB_RECURSE LIB_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

# ------------------------------------------------------------
# Library definition
# ------------------------------------------------------------
set(LIB_VERSION 1.0.0)
add_library(crobj SHARED ${LIB_SOURCES})

set_target_properties(crobj PROPERTIES
    VERSION ${LIB_VERSION}
    SOVERSION 1
    OUTPUT_NAME "crobj"
    PUBLIC_HEADER ""
    C_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
)

# ------------------------------------------------------------
# Linking
# ------------------------------------------------------------
if(ENABLE_LVGL_BACKEND)
    target_link_libraries(crobj
        ${LVGL_LIBRARIES}
        ${LIBDRM_LIBRARIES}
    )
endif()

# ------------------------------------------------------------
# Exported headers
# ------------------------------------------------------------
target_include_directories(crobj
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# ------------------------------------------------------------
# Installation rules
# ------------------------------------------------------------
include(GNUInstallDirs)

install(TARGETS crobj
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/crobj/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/crobj
)

# ------------------------------------------------------------
# Status summary
# ------------------------------------------------------------
message(STATUS "")
message(STATUS "----------------------------------------")
message(STATUS " Cronos UI Library Configuration Summary")
message(STATUS "----------------------------------------")
message(STATUS "  LVGL backend:     ${ENABLE_LVGL_BACKEND}")
message(STATUS "  Shared library:   ${BUILD_SHARED_LIBS}")
message(STATUS "  C standard:       ${CMAKE_C_STANDARD}")
message(STATUS "  Install prefix:   ${CMAKE_INSTALL_PREFIX}")
message(STATUS "----------------------------------------")
message(STATUS "")
