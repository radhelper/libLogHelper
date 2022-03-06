# example https://stackoverflow.com/questions/17511496/how-to-create-a-shared-library-with-cmake

cmake_minimum_required(VERSION 3.16)
project(LogHelper LANGUAGES C VERSION 1.0.0 DESCRIPTION "Log Helper for radiation experiments")

set(CMAKE_C_STANDARD 11)

include(GNUInstallDirs)
set(CMAKE_BUILD_TYPE Release)

add_library(
        LogHelper
        SHARED
        src/log_helper.c
)

set_target_properties(LogHelper PROPERTIES VERSION ${PROJECT_VERSION})

set_target_properties(LogHelper PROPERTIES PUBLIC_HEADER include/log_helper.h)

configure_file(logHelper.pc.in logHelper.pc @ONLY)
target_include_directories(LogHelper PRIVATE include)

install(TARGETS LogHelper
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(LogHelper)

install(FILES ${CMAKE_BINARY_DIR}/logHelper.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

add_subdirectory(examples)

# Define debug feature
option(WITH_DEBUG "To print debug information" OFF)

if(WITH_DEBUG)
    add_definitions(-DDEBUG=1)
endif()
