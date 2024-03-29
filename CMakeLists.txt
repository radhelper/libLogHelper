cmake_minimum_required(VERSION 3.16)

# Common CMake settings
SET(PACKAGE_VERSION 1.1.1)
project(LogHelper LANGUAGES C CXX VERSION ${PACKAGE_VERSION} DESCRIPTION "Log Helper for radiation experiments")

########################################################################################################################
#In many cases you will want to make sure that the required libraries are always found independent
#from LD_LIBRARY_PATH and the install location. use, i.e. don't skip the full RPATH for the build tree
set(CMAKE_SKIP_BUILD_RPATH FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# the RPATH to be used when installing, but only if it's not a system directory
list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
if ("${isSystemDir}" STREQUAL "-1")
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
endif ("${isSystemDir}" STREQUAL "-1")
########################################################################################################################

#set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)

# Selects between C++ and C backend
option(WITH_CXX_BACKEND "To use C++ over C backend" ON)
if (NOT WITH_CXX_BACKEND)
    message(WARNING "Option for C backend was disabled on libLogHelper ${PACKAGE_VERSION}, using C++ backend")
endif ()
# Define debug feature
option(WITH_DEBUG "To print debug information" ON)
# PYTHON Wrapper
option(WITH_PYTHON_SUPPORT "To build the SWIG Python 3 wrapper" ON)

# Include directories for the lib
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(
        LogHelper
        SHARED
        src/log_helper.cpp
        src/log_helper_c_wrapper.cpp
        src/file_writer.cpp
        include/log_helper.h
        include/log_helper.hpp
)

########################################################################################################################
# Set the logging approach
# UPDATE IF THERE IS CHANGES on common.hpp
#    enum class LoggingType{
#        LOCAL_ONLY = 0,
#        UDP_ONLY,
#        LOCAL_AND_UDP
#    } ;
if (NOT LOGGING_TYPE OR LOGGING_TYPE STREQUAL "LOCAL")
    message("-- Selecting LOCAL logging approach")
    add_definitions(-DLOGGING_TYPE=0)
elseif (LOGGING_TYPE STREQUAL "UDP")
    message("-- Selecting UDP logging approach")
    add_definitions(-DLOGGING_TYPE=1)
elseif (LOGGING_TYPE STREQUAL "LOCAL_AND_UDP")
    message("-- Selecting LOCAL_AND_UPD logging approach")
    add_definitions(-DLOGGING_TYPE=2)
else ()
    message(FATAL_ERROR "INVALID LOGGING_TYPE CONFIGURATION, USE: LOCAL, UPD, LOCAL_AND_UDP")
endif ()


########################################################################################################################
# CONFIGURE INSTALLATION
# Set the pkg-config files

include(GNUInstallDirs)
set(CMAKE_BUILD_TYPE Release)

set_target_properties(LogHelper PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(LogHelper PROPERTIES PUBLIC_HEADER "include/log_helper.h;include/log_helper.hpp")

configure_file(cmake_install_scripts/LogHelper.pc.in LogHelper.pc @ONLY)
target_include_directories(LogHelper PRIVATE include)

install(TARGETS LogHelper
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

target_link_libraries(LogHelper)

install(FILES ${CMAKE_BINARY_DIR}/LogHelper.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

# CONFIGURE log-helper.conf file #######################################
# Path to the radiation-benchmarks repository
set(HOME_DIR $ENV{HOME})

# Set radiation-benchmarks directory to keep compatibility with older setup
if (NOT RAD_BENCHS_INSTALL_DIR)
    set(RAD_BENCHS_INSTALL_DIR ${HOME_DIR}/radiation-benchmarks)
endif ()
# Killall signal command to be sent to the SW watchdog
if (NOT WATCHDOG_COMMANDS)
    set(WATCHDOG_COMMANDS "none")
endif ()
# System tmp dir
if (NOT TMP_DIR)
    set(TMP_DIR /tmp)
endif ()
# Path to file that will contains 1/0 that refers to ECC enabled or disabled respectively.
if (NOT ECC_INFO_FILE_DIR)
    set(ECC_INFO_FILE_DIR /tmp/ecc-info-file.txt)
endif ()
# Server that will receive the messages IP
if (NOT SERVER_IP)
    set(SERVER_IP 192.168.1.5)
endif ()
# Server port that will receive the messages
if (NOT SERVER_PORT)
    set(SERVER_PORT 1024)
endif ()

# Set the log dir for the benchmarks
# Default is /var/radiation-benchmarks
if (NOT LOG_DIR)
    set(LOG_DIR "/${CMAKE_INSTALL_LOCALSTATEDIR}")
endif ()
########################################################################################################################
# Installation process
set(INSTALL_SCRIPTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cmake_install_scripts)
# configuration file that will be read by the library
set(RADIATION_CONF_FILE radiation-benchmarks.conf)
configure_file(${INSTALL_SCRIPTS_DIR}/${RADIATION_CONF_FILE}.in ${RADIATION_CONF_FILE} @ONLY)
add_definitions(-DCONFIG_FILE_PATH=\"/${CMAKE_INSTALL_SYSCONFDIR}/${RADIATION_CONF_FILE}\")
install(
        FILES ${CMAKE_BINARY_DIR}/${RADIATION_CONF_FILE}
        DESTINATION /${CMAKE_INSTALL_SYSCONFDIR}
)

# Destination of the atBoot.sh file
include(ProcessorCount)
ProcessorCount(NUMBER_OF_PROCESSORS)
configure_file(${INSTALL_SCRIPTS_DIR}/atBoot.sh.in atBoot.sh @ONLY)
install(
        FILES ${CMAKE_BINARY_DIR}/atBoot.sh
        DESTINATION ${HOME_DIR}
)

# Destination of the rc.local file
configure_file(${INSTALL_SCRIPTS_DIR}/rc.local.in rc.local @ONLY)
install(
        FILES ${CMAKE_BINARY_DIR}/rc.local
        DESTINATION /${CMAKE_INSTALL_SYSCONFDIR}
)

# Post install script commands
set(SUDOERS_ADD \"$ENV{USER}\ ALL=NOPASSWD:/sbin/reboot\")
install(CODE
        "execute_process(COMMAND ${INSTALL_SCRIPTS_DIR}/post_install.sh
        ${LOG_DIR} ${HOME_DIR} ${SUDOERS_ADD})")
# uninstall target
if (NOT TARGET uninstall)
    configure_file(
            "${INSTALL_SCRIPTS_DIR}/Uninstall.cmake.in"
            "${CMAKE_CURRENT_BINARY_DIR}/Uninstall.cmake"
            IMMEDIATE @ONLY)

    add_custom_target(uninstall
            COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/Uninstall.cmake)
endif ()

########################################################################################################################

# Debug feature
if (WITH_DEBUG)
    add_definitions(-DDEBUG=1)
endif ()

# Python wrapper
if (WITH_PYTHON_SUPPORT)
    # TODO: fix this part with an approach similar to this https://github.com/Mizux/cmake-swig
    message("-- Building with python support, run the following command before run cmake:
             -- sudo apt install python3 python3-dev python3-distutils python3-venv swig")
    find_package(SWIG "4.0.1" REQUIRED)
    include(UseSWIG)
    set(UseSWIG_TARGET_NAME_PREFERENCE STANDARD)

    find_package(Python 3 COMPONENTS Interpreter Development REQUIRED)
    list(APPEND CMAKE_SWIG_FLAGS "-python" "-py3" "-DPY3")

    set_property(SOURCE python/log_helper.i PROPERTY CPLUSPLUS ON)
    set_property(SOURCE python/log_helper.i PROPERTY SWIG_MODULE_NAME log_helper)

    swig_add_library(
            log_helper
            TYPE MODULE
            LANGUAGE python
            OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}
            SOURCES python/log_helper.i
    )
    add_library(${PROJECT_NAMESPACE}::log_helper ALIAS log_helper)

    target_include_directories(log_helper
            PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/include
            ${Python3_INCLUDE_DIRS}
    )
    set_property(TARGET log_helper PROPERTY SWIG_USE_TARGET_INCLUDE_DIRECTORIES ON)

    set_property(
            TARGET log_helper
            PROPERTY INCLUDE_DIRECTORIES ${Python_INCLUDE_DIRS}
            ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

    TARGET_LINK_LIBRARIES(log_helper ${PYTHON_LIBRARIES})
    TARGET_LINK_LIBRARIES(log_helper LogHelper)
    find_program(PYTHON "python3")
    #    set(SETUP_PY_IN "${CMAKE_CURRENT_SOURCE_DIR}/python/setup.py.in")
    #    set(SETUP_PY    "${CMAKE_CURRENT_BINARY_DIR}/setup.py")
    #    set(DEPS        "${CMAKE_CURRENT_SOURCE_DIR}/python/log_helper/__init__.py")
    #    set(OUTPUT      "${CMAKE_CURRENT_BINARY_DIR}/build/timestamp")
    #
    #    configure_file(${SETUP_PY_IN} ${SETUP_PY})
    #    add_custom_command(OUTPUT ${OUTPUT}
    #            COMMAND ${PYTHON} ${SETUP_PY} build
    #            COMMAND ${CMAKE_COMMAND} -E touch ${OUTPUT}
    #            DEPENDS ${DEPS})
    #
    #    add_custom_target(target ALL DEPENDS ${OUTPUT})
    #
    #    install(CODE "execute_process(COMMAND ${PYTHON} ${SETUP_PY} install)")
endif ()

########################################################################################################################
# Build examples
add_subdirectory(examples)
