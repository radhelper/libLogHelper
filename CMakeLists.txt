# example https://stackoverflow.com/questions/17511496/how-to-create-a-shared-library-with-cmake
cmake_minimum_required(VERSION 3.16)

# Common CMake settings
project(LogHelper LANGUAGES C CXX VERSION 1.0.0 DESCRIPTION "Log Helper for radiation experiments")

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)

# Selects between C++ and C backend
option(WITH_CXX_BACKEND "To use C++ over C backend" ON)
# Define debug feature
option(WITH_DEBUG "To print debug information" ON)
# PYTHON Wrapper
option(WITH_PYTHON_SUPPORT "To build the SWIG Python 3.8 wrapper" ON)

# Include directories for the lib
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

if (WITH_CXX_BACKEND)
    add_library(
            LogHelper
            SHARED
            src/log_helper.cpp
    )
else ()
    add_library(
            LogHelper
            SHARED
            src/log_helper.c
    )
endif ()

include(GNUInstallDirs)
set(CMAKE_BUILD_TYPE Release)


set_target_properties(LogHelper PROPERTIES VERSION ${PROJECT_VERSION})

set_target_properties(LogHelper
        PROPERTIES PUBLIC_HEADER include/log_helper.h)
##############################################################################
# CONFIGURE INSTALLATION
# Set the pkg-config files
configure_file(LogHelper.pc.in LogHelper.pc @ONLY)
target_include_directories(LogHelper PRIVATE include)

install(TARGETS LogHelper
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

target_link_libraries(LogHelper)

install(FILES ${CMAKE_BINARY_DIR}/LogHelper.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

# CONFIGURE log-helper.conf file #######################################
# Path to the radiation-benchmarks repository
if(NOT RAD_BENCHS_INSTALL_DIR)
    set(RAD_BENCHS_INSTALL_DIR /home/carol/radiation-benchmarks)
endif()
# Killall signal command to be sent to the SW watchdog
if(NOT WATCHDOG_COMMANDS)
    set(WATCHDOG_COMMANDS
            "killall -q -USR1 killtestSignal-2.0.py; killall -q -USR1 test_killtest_commands_json-2.0.py; killall -q -USR1 python3")
endif()
# System tmp dir
if(NOT TMP_DIR)
    set(TMP_DIR /tmp)
endif()
# Path to file that will contains 1/0 that refers to ECC enabled or disabled respectively.
if(NOT ECC_INFO_FILE_DIR)
    set(ECC_INFO_FILE_DIR /tmp/ecc-info-file.txt)
endif()
# Server that will receive the messages IP
if (NOT SERVER_IP)
    set(SERVER_IP 192.168.1.5)
endif ()
# Server port that will receive the messages
if (NOT SERVER_PORT)
    set(SERVER_PORT 1024)
endif ()

set(RADIATION_CONF_FILE radiation-benchmarks.conf)
configure_file(${RADIATION_CONF_FILE}.in ${RADIATION_CONF_FILE} @ONLY)
add_definitions(-DCONFIG_FILE_PATH="/${CMAKE_INSTALL_SYSCONFDIR}/${RADIATION_CONF_FILE}")
install(
        FILES ${CMAKE_BINARY_DIR}/${RADIATION_CONF_FILE}
        DESTINATION /${CMAKE_INSTALL_SYSCONFDIR}
)
##############################################################################

# Build examples
add_subdirectory(examples)

# Debug feature
if (WITH_DEBUG)
    add_definitions(-DDEBUG=1)
endif ()

# Python wrapper
if (WITH_PYTHON_SUPPORT)
    find_package(SWIG REQUIRED)
    include(UseSWIG)
    set(UseSWIG_TARGET_NAME_PREFERENCE STANDARD)

    swig_add_library(log_helper
            TYPE SHARED
            LANGUAGE python
            OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}
            SOURCES python_wrapper/log_helper.i
            )

    find_package(Python 3.8 COMPONENTS Interpreter Development.Module REQUIRED)
    set_property(
            TARGET log_helper
            PROPERTY INCLUDE_DIRECTORIES ${Python_INCLUDE_DIRS}
            ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

    set_property(
            SOURCE python_wrapper/log_helper.i
            PROPERTY SWIG_FLAGS "-py3" "-builtin" "-extranative"
    )

    TARGET_LINK_LIBRARIES(log_helper ${PYTHON_LIBRARIES})
    TARGET_LINK_LIBRARIES(log_helper LogHelper)
endif ()
