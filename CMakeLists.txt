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
option(BUILD_SHARED_LIBS "Build shared library (.so)" ON)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Conservative warnings (Yocto friendly)
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
# Sources
# ------------------------------------------------------------
# NOTE: Avoid GLOB for reproducible builds (Yocto best practice)
set(CROBJ_SOURCES
    src/crobj_align.c
    src/crobj_border.c
    src/crobj_core.c
    src/crobj_creator.c
    src/crobj_mgr.c
    src/crobj_padding.c
    src/crobj_rotation.c
    src/crobj_size.c

    src/layouts/crobj_flex.c
    src/layouts/crobj_grid.c

    src/widget/crobj_widget_common.c
    src/widget/crobj_menu.c
    src/widget/crobj_keyboard.c
)

add_library(crobj SHARED ${CROBJ_SOURCES})

set_target_properties(crobj PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    C_VISIBILITY_PRESET default
    VISIBILITY_INLINES_HIDDEN NO
)

# ------------------------------------------------------------
# Includes
# ------------------------------------------------------------
target_include_directories(crobj
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

if(ENABLE_LVGL_BACKEND)
    target_include_directories(crobj PRIVATE
        ${LVGL_INCLUDE_DIRS}
        ${LIBDRM_INCLUDE_DIRS}
    )
endif()

# ------------------------------------------------------------
# Linking
# ------------------------------------------------------------
if(ENABLE_LVGL_BACKEND)
    target_link_libraries(crobj PRIVATE
        ${LVGL_LIBRARIES}
        ${LIBDRM_LIBRARIES}
    )
endif()

# ------------------------------------------------------------
# Installation rules
# ------------------------------------------------------------
include(GNUInstallDirs)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/libcrobj.pc.in
    ${CMAKE_CURRENT_BINARY_DIR}/libcrobj.pc
    @ONLY
)

install(TARGETS crobj
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# Install public headers
install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/crobj
)

# Install pkg-config file
install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/libcrobj.pc
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)

# ------------------------------------------------------------
# Summary
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
