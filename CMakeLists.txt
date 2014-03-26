######################
#
#    Copyright (C) 2014 EngSaS - Engineering Solutions and Services Langenbach. All rights reserved.
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
######################

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(qgv)

SET(TARGET_VERSION_MAJOR 0 )
    SET(TARGET_VERSION_MINOR 1 )
    SET(${PROJECT_NAME}_PATCH_LEVEL   0 )
SET(${PROJECT_NAME}_SOVERSION ${TARGET_VERSION_MAJOR})
SET(${PROJECT_NAME}_VERSION
    "${TARGET_VERSION_MAJOR}.${TARGET_VERSION_MINOR}.${${PROJECT_NAME}_PATCH_LEVEL}" )

# INCLUDE(CTest)

# Extending the Module path with the local module path of desuto
SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules )

OPTION(BUILD_DEBUG "Build in debug mode" ON)
OPTION(BINDINGS_QT4 "Build with Qt4" OFF)
OPTION(BINDINGS_QT5 "Build with Qt5" OFF)
SET(BUILD_DEV ON)

SET(BUILD_DEBUG_WANTED ${BUILD_DEBUG})
SET(BUILD_DEV_WANTED ${BUILD_DEV})
SET(BINDINGS_QT4_WANTED ${BINDINGS_QT4})
SET(BINDINGS_QT5_WANTED ${BINDINGS_QT5})

IF(BUILD_DEBUG)
	SET(CMAKE_BUILD_TYPE Debug)
ELSE()
	SET(CMAKE_BUILD_TYPE Release)
ENDIF()

IF(NOT BINDINGS_QT4 AND NOT BINDINGS_QT5)
    MESSAGE("Trying to autodetect Qt version")
    FIND_PACKAGE(Qt4 QUIET)
    IF(Qt4_FOUND OR QT4_FOUND)
        SET(BINDINGS_QT4 ON)
        MESSAGE("Found Qt4 --> Prefer Qt4 over Qt5")
    ELSE(Qt4_FOUND OR QT4_FOUND)
        SET(BINDINGS_QT5 ON)
        MESSAGE("Qt4 not found --> Try to build with Qt5")
    ENDIF(Qt4_FOUND OR QT4_FOUND)
ENDIF(NOT BINDINGS_QT4 AND NOT BINDINGS_QT5)
IF(BINDINGS_QT4)
        # Qt4 must be included after setting the build type
        FIND_PACKAGE(Qt4 REQUIRED)
ENDIF(BINDINGS_QT4)
IF(BINDINGS_QT5)
        IF(BINDINGS_QT4)
                MESSAGE("Qt4 and Qt5 enabled --> Preferring Qt4 and therefore disabling Qt5")
                SET(BINDINGS_QT5  OFF)
        ELSE(BINDINGS_QT4)
                FIND_PACKAGE(Qt5Core REQUIRED)
                FIND_PACKAGE(Qt5Widgets REQUIRED)
                FIND_PACKAGE(Qt5Xml REQUIRED)
                FIND_PACKAGE(Qt5XmlPatterns REQUIRED)
                FIND_PACKAGE(Qt5Sql REQUIRED)
                FIND_PACKAGE(Qt5Network REQUIRED)
                FIND_PACKAGE(Qt5LinguistTools REQUIRED)
                # Do not automatically link against qtmain
                CMAKE_POLICY(SET CMP0020 OLD)
        ENDIF(BINDINGS_QT4)
ENDIF(BINDINGS_QT5)
# enable automatic moc by CMAKE
SET(CMAKE_AUTOMOC ON)

IF(NOT KDE)
	IF(NOT APPLE)
		SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-undefined")
	ENDIF(NOT APPLE)
	IF(MSVC)
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DQT_NO_EXCEPTIONS -Zc:wchar_t-")
	ELSE(MSVC)
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wnon-virtual-dtor -Wno-long-long -Wundef -Wcast-align -Wchar-subscripts -Wall -W -Wpointer-arith -Wformat-security -fno-exceptions -DQT_NO_EXCEPTIONS -fno-check-new -fno-common")
	ENDIF(MSVC)
	IF(NOT WIN32 AND NOT APPLE)
#		SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--fatal-warnings -lc -Wl,--disable-auto-import")
		SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--fatal-warnings -lc")
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ansi")
	ENDIF(NOT WIN32 AND NOT APPLE)
ENDIF(NOT KDE)
IF(BUILD_DEBUG AND NOT MSVC)
	SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
ENDIF(BUILD_DEBUG AND NOT MSVC)
ADD_DEFINITIONS(${QT_DEFINITIONS})

# directory to install binaries
SET(bin_DIR ${CMAKE_INSTALL_PREFIX}/bin)
IF(WIN32)
	SET(bin_DIR .)
ENDIF(WIN32)

IF(NOT WIN32)
	SET(LIB_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/lib/" CACHE STRING "Where to install the library files to")
ENDIF(NOT WIN32)
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
IF(UNIX)
#  SET(designer_DIR "/usr/lib/qt4/plugins/designer")
	SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
	SET(include_DIR ${CMAKE_INSTALL_PREFIX}/include/${PROJECT_NAME})
	SET(find_cmake_DIR ${CMAKE_ROOT}/Modules/)
ELSE(UNIX)
	SET(LIB_INSTALL_DIR ${bin_DIR})
	SET(LIBRARY_OUTPUT_PATH ${EXECUTABLE_OUTPUT_PATH})
	SET(include_DIR ${bin_DIR}/include/${PROJECT_NAME})
	SET(find_cmake_DIR ${CMAKE_ROOT}/Modules/)
ENDIF(UNIX)

IF(WIN32)
  IF(BUILD_DEBUG)
    SET(qt_LIBS ${qt_LIBS}/Debug)
  ELSE(BUILD_DEBUG)
    SET(qt_LIBS ${qt_LIBS}/Release)
  ENDIF(BUILD_DEBUG)
ENDIF()

FIND_PACKAGE(Qt4 REQUIRED)
ADD_DEFINITIONS(${QT_DEFINITIONS})
FIND_PACKAGE(Graphviz REQUIRED)

######################
#
# include source tree
#
######################
SET(libqgv_SOURCE_DIR ${CMAKE_SOURCE_DIR}/QGVCore)
SET(libqgv_BINARY_DIR ${CMAKE_BINARY_DIR}/QGVCore)
ADD_SUBDIRECTORY(QGVCore)
ADD_SUBDIRECTORY(Sample)

######################
#
# printing choosen configuration
#
######################
MESSAGE("######################################################")
MESSAGE("")
MESSAGE("CONFIGURATION")
MESSAGE("")
MESSAGE("CMAKE_SHARED_LINKER_FLAGS: ${CMAKE_SHARED_LINKER_FLAGS}")
MESSAGE("CMAKE_CXX_FLAGS:           ${CMAKE_CXX_FLAGS}")
MESSAGE("CMAKE_INSTALL_PREFIX:      ${CMAKE_INSTALL_PREFIX}")
MESSAGE("")
MESSAGE("OPTIONS:")
MESSAGE("  Name                               Wanted     Used")
MESSAGE("  BUILD_PACKAGE:                     ${BUILD_PACKAGE_WANTED}	${BUILD_PACKAGE}")
MESSAGE("  BUILD_DEBUG:                       ${BUILD_DEBUG_WANTED}		${BUILD_DEBUG}")
MESSAGE("  BUILD_TESTING:                     ${BUILD_TESTING_WANTED}		${BUILD_TESTING}")
MESSAGE("  BUILD_DEV:                         ${BUILD_DEV}	${BUILD_DEV}")
MESSAGE("  WITH_DAEMON:                       ${WITH_DAEMON_WANTED}	${WITH_DAEMON}")
MESSAGE("  WITH_STANDARDS:                    ${WITH_STANDARDS_WANTED}		${WITH_STANDARDS}")
MESSAGE("  WITH_STRENGTH:                     ${WITH_STRENGTH_WANTED}	${WITH_STRENGTH}")
MESSAGE("  WITH_MORPHBOX:                     ${WITH_MORPHBOX_WANTED}	${WITH_MORPHBOX}")
MESSAGE("  WITH_CE_WANTED:                    ${WITH_CE_WANTED}	${WITH_CE}")
MESSAGE("  WITH_CONSTRUCTIONKIT:              ${WITH_CONSTRUCTIONKIT_WANTED}	${WITH_CONSTRUCTIONKIT}")
MESSAGE("  WITH_REGMAN:                       ${WITH_REGMAN_WANTED}		${WITH_REGMAN}")
MESSAGE("  WITH_FUNCMO:                       ${WITH_FUNCMO_WANTED}		${WITH_FUNCMO}")
MESSAGE("  WITH_COMP_BINARY:                  ${WITH_COMP_BINARY_WANTED}		${WITH_COMP_BINARY}")
MESSAGE("  WITH_COMP_WEPO:                    ${WITH_COMP_WEPO_WANTED}		${WITH_COMP_WEPO}")
MESSAGE("  L10N_ALL:                          ${L10N_ALL_WANTED}		${L10N_ALL}")
MESSAGE("    L10N_DE:                         ${L10N_DE_WANTED}		${L10N_DE}")
MESSAGE("    L10N_EN:                         ${L10N_EN_WANTED}		${L10N_EN}")
MESSAGE("    L10N_FR:                         ${L10N_FR_WANTED}		${L10N_FR}")
MESSAGE("")
MESSAGE("######################################################")

######################
#
# Add uninstall target
#
######################
CONFIGURE_FILE(
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/cmake/cmake_uninstall.cmake"
	IMMEDIATE @ONLY)
ADD_CUSTOM_TARGET(uninstall
	"${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake/cmake_uninstall.cmake")


######################
#
# CPACK
#
######################
SET(CPACK_GENERATOR "NSIS")
SET(CPACK_PACKAGE_NAME "QGV")
SET(CPACK_PACKAGE_VENDOR "")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Qt interface for Graphviz")
SET(CPACK_PACKAGE_VERSION ${${PROJECT_NAME}_VERSION})
SET(CPACK_PACKAGE_VERSION_MAJOR ${TARGET_MAJOR_VERSION})
SET(CPACK_PACKAGE_VERSION_MINOR ${TARGET_MINOR_VERSION})
SET(CPACK_PACKAGE_VERSION_PATCH ${${PROJECT_NAME}_PATCH_LEVEL})
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "qgv")

# add all components which should be installable via the package
IF(WIN32)
  SET(external_DLL
    qtlibs
  )
ENDIF(WIN32)

SET(CPACK_COMPONENTS_ALL
  library
  ${external_DLL}
	sample
)

# License file
SET(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/LICENSE.txt)

# Pre selected installation profiles
SET(CPACK_ALL_INSTALL_TYPES Full Client Daemon)

# groups for components
SET(CPACK_COMPONENT_GROUP_BASE_DISPLAY_NAME "Base")
SET(CPACK_COMPONENT_GROUP_BASE_DESCRIPTION "The base components needed by most installation scenarios")
IF(WITH_DAEMON)
  SET(CPACK_COMPONENT_GROUP_DAEMON_DISPLAY_NAME "Daemon")
  SET(CPACK_COMPONENT_GROUP_DAEMON_DESCRIPTION "This components provide stuff which needs background running")
ENDIF(WITH_DAEMON)
SET(CPACK_COMPONENT_GROUP_MODULES_DISPLAY_NAME "Modules")
SET(CPACK_COMPONENT_GROUP_MODULES_DESCRIPTION "The modules which can be used with DeSuTo")
SET(CPACK_COMPONENT_GROUP_3RDPARTYLIBS_DISPLAY_NAME "3rd Party Libraries")
SET(CPACK_COMPONENT_GROUP_3RDPARTYLIBS_DESCRIPTION "3rd Party libraries which are needed to run DeSuTo and or modules. They are only needed if they are not already exist in PATH")


# the scheme of the variable names are: CPACK_COMPNONENT_%COMPONENTNAMEINUPPERCASE%_DISPLAY_NAME
# where %COMPONENTNAMEINUPPERCASE% must be replaced by the upper cased name of the component
# see below for example
SET(CPACK_COMPONENT_LIBRARY_DISPLAY_NAME "LibDeSuTo")
SET(CPACK_COMPONENT_LIBRARY_DESCRIPTION
   "The library of the DeSuTo project provides components which are used by every module and the applications")
SET(CPACK_COMPONENT_LIBRARY_GROUP "Base")
SET(CPACK_COMPONENT_LIBRARY_INSTALL_TYPES Full Client Daemon)

SET(CPACK_COMPONENT_QTLIBS_DISPLAY_NAME "QT")
SET(CPACK_COMPONENT_QTLIBS_DESCRIPTION
   "Libraries which are needed, but not belonging to this project. They must not be installed, if they are already installed at path")
SET(CPACK_COMPONENT_QTLIBS_GROUP "3rdPartyLibs")
SET(CPACK_COMPONENT_QTLIBS_INSTALL_TYPES Full Client Daemon)

SET(CPACK_COMPONENT_MYSQLLIBS_DISPLAY_NAME "Mysql-QT-Plugin")
SET(CPACK_COMPONENT_MYSQLLIBS_DESCRIPTION
   "Libraries which are needed, but not belonging to this project. This libs are needed, if you want to connect to a MySQL-Server")
SET(CPACK_COMPONENT_MYSQLLIBS_GROUP "3rdPartyLibs")
SET(CPACK_COMPONENT_MYSQLLIBS_INSTALL_TYPES Full Client Daemon)

SET(CPACK_COMPONENT_APPLICATION_DISPLAY_NAME "DeSuTo Application")
SET(CPACK_COMPONENT_APPLICATION_DESCRIPTION
   "The main application of the DeSuTo application. It provides a GUI-frame for all modules")
SET(CPACK_COMPONENT_APPLICATION_GROUP "Base")
SET(CPACK_COMPONENT_APPLICATION_DEPENDS library)
SET(CPACK_COMPONENT_APPLICATION_INSTALL_TYPES Full Client)

IF(WITH_DAEMON)
  SET(CPACK_COMPONENT_SYSTRAY_DISPLAY_NAME "DeSuToDaeomen")
  SET(CPACK_COMPONENT_SYSTRAY_DESCRIPTION
    "System tray tool to run some tasks at the background")
  SET(CPACK_COMPONENT_SYSTRAY_GROUP "Daemon")
  SET(CPACK_COMPONENT_SYSTRAY_DEPENDS library)
  SET(CPACK_COMPONENT_SYSTRAY_INSTALL_TYPES Full Daemon)
ENDIF(WITH_DAEMON)

IF(WITH_CE)
  SET(CPACK_COMPONENT_MODULECE_DISPLAY_NAME "DeSuToCE")
  SET(CPACK_COMPONENT_MODULECE_DESCRIPTION
    "Module to help with CE-Marking")
  SET(CPACK_COMPONENT_MODULECE_GROUP "Modules")
  SET(CPACK_COMPONENT_MODULECE_DEPENDS library)
  SET(CPACK_COMPONENT_MODULECE_INSTALL_TYPES Full Client Daemon)
ENDIF(WITH_CE)

IF(WITH_CONSTRUCTIONKIT)
  SET(CPACK_COMPONENT_MODULECONSTRUCTIONKIT_DISPLAY_NAME "DeSuToConstructionKit")
  SET(CPACK_COMPONENT_MODULECONSTRUCTIONKIT_DESCRIPTION
     "Module to create construction kits")
  SET(CPACK_COMPONENT_MODULECONSTRUCTIONKIT_GROUP "Modules")
  SET(CPACK_COMPONENT_MODULECONSTRUCTIONKIT_DEPENDS library)
  SET(CPACK_COMPONENT_MODULECONSTRUCTIONKIT_INSTALL_TYPES Full Client)
ENDIF(WITH_CONSTRUCTIONKIT)

IF(WITH_STANDARDS)
	SET(CPACK_COMPONENT_MODULESTANDARDS_DISPLAY_NAME "Standards Management")
  SET(CPACK_COMPONENT_MODULESTANDARDS_DESCRIPTION
     "Manage standards like DIN, DIN EN, DIN ISO, ISO, ...")
  SET(CPACK_COMPONENT_MODULESTANDARDS_GROUP "Modules")
  SET(CPACK_COMPONENT_MODULESTANDARDS_DEPENDS library)
  SET(CPACK_COMPONENT_MODULESTANDARDS_INSTALL_TYPES Full Client Daemon)
ENDIF(WITH_STANDARDS)

IF(WITH_REGMAN)
  SET(CPACK_COMPONENT_MODULEREGMAN_DISPLAY_NAME "Registry Manager")
  SET(CPACK_COMPONENT_MODULEREGMAN_DESCRIPTION
     "Support's the engineer developing registers.")
  SET(CPACK_COMPONENT_MODULEREGMAN_GROUP "Modules")
  SET(CPACK_COMPONENT_MODULEREGMAN_DEPENDS library)
  SET(CPACK_COMPONENT_MODULEREGMAN_INSTALL_TYPES Full Client)
ENDIF(WITH_REGMAN)

IF(WITH_INTERNAL_PDF_VIEWER)
  SET(CPACK_COMPONENT_POPPLER_DISPLAY_NAME "Poppler")
  SET(CPACK_COMPONENT_POPPLER_DESCRIPTION
	"Libraries which are needed, but not belonging to this project. The must not be installed, if they are already installed at path")
  SET(CPACK_COMPONENT_POPPLER_GROUP "3rdPartyLibs")
  SET(CPACK_COMPONENT_POPPLER_INSTALL_TYPES Full Client Daemon)
ENDIF(WITH_INTERNAL_PDF_VIEWER)

IF(WITH_STRENGTH)
  SET(CPACK_COMPONENT_MODULESTRENGTH_DISPLAY_NAME "DeSuToStrength")
  SET(CPACK_COMPONENT_MODULESTRENGTH_DESCRIPTION
     "Allows to calculate some machine elements")
  SET(CPACK_COMPONENT_MODULESTRENGTH_GROUP "Modules")
  SET(CPACK_COMPONENT_MODULESTRENGTH_DEPENDS library)
  SET(CPACK_COMPONENT_MODULESTRENGTH_INSTALL_TYPES Full Client)
ENDIF(WITH_STRENGTH)

IF(WITH_MORPHBOX)
	SET(CPACK_COMPONENT_MODULEMORPHBOX_DISPLAY_NAME "Morphological Boxes")
	SET(CPACK_COMPONENT_MODULEMORPHBOX_DESCRIPTION
		 "Supports the use of morphological boxes to find many good variants.")
	SET(CPACK_COMPONENT_MODULEMORPHBOX_GROUP "Modules")
	SET(CPACK_COMPONENT_MODULEMORPHBOX_DEPENDS library)
	SET(CPACK_COMPONENT_MODULEMORPHBOX_INSTALL_TYPES Full Client)
ENDIF(WITH_MORPHBOX)

IF(WITH_FUNCMO)
	SET(CPACK_COMPONENT_MODULEFUNCMO_DISPLAY_NAME "Function Modeler")
	SET(CPACK_COMPONENT_MODULEFUNCMO_DESCRIPTION
		 "Allows to model the function structure of your development object.")
	SET(CPACK_COMPONENT_MODULEFUNCMO_GROUP "Modules")
	SET(CPACK_COMPONENT_MODULEFUNCMO_DEPENDS library)
	SET(CPACK_COMPONENT_MODULEFUNCMO_INSTALL_TYPES Full Client)
ENDIF(WITH_FUNCMO)

IF(WITH_COMP_BINARY)
	SET(CPACK_COMPONENT_MODULECOMPBINARY_DISPLAY_NAME "Binary Comparison")
	SET(CPACK_COMPONENT_MODULECOMPBINARY_DESCRIPTION
		 "Compare several alterantives (called options in DeSuTo) easily and fast with binary values.")
	SET(CPACK_COMPONENT_MODULECOMPBINARY_GROUP "Modules")
	SET(CPACK_COMPONENT_MODULECOMPBINARY_DEPENDS library)
	SET(CPACK_COMPONENT_MODULECOMPBINARY_INSTALL_TYPES Full Client)
ENDIF(WITH_COMP_BINARY)

IF(WITH_COMP_WEPO)
	SET(CPACK_COMPONENT_MODULECOMPWEPO_DISPLAY_NAME "Weighted Points Comparisons")
	SET(CPACK_COMPONENT_MODULECOMPWEPO_DESCRIPTION
		 "Compare several alterantives (called options in DeSuTo) detailed with weighted points.")
	SET(CPACK_COMPONENT_MODULECOMPWEPO_GROUP "Modules")
	SET(CPACK_COMPONENT_MODULECOMPWEPO_DEPENDS library)
	SET(CPACK_COMPONENT_MODULECOMPWEPO_INSTALL_TYPES Full Client)
ENDIF(WITH_COMP_WEPO)

# used by NSIS to create startmenu links to the executables
# this entrys are only useful if a bin folder is used, otherwise use the empty EXECUTABLES
# and fill out the NSIS_MENU_LINKS
#SET(CPACK_PACKAGE_EXECUTABLES ${CPACK_PACKAGE_EXECUTABLES} "desuto" "DeSuTo")
#SET(CPACK_PACKAGE_EXECUTABLES ${CPACK_PACKAGE_EXECUTABLES} "desutodaemon" "DeSuToDaemon")
SET(CPACK_PACKAGE_EXECUTABLES "" "")

SET(CPACK_NSIS_MENU_LINKS "epartexe.exe \\\"desuto\\\"" "DeSuTo")
IF(WITH_DAEMON)
  SET(CPACK_NSIS_MENU_LINKS ${CPACK_NSIS_MENU_LINKS} "desutodaemon.exe" "DeSuToDaemon")
ENDIF(WITH_DAEMON)
# IF(WITH_REGMAN)
	# SET(CPACK_NSIS_MENU_LINKS ${CPACK_NSIS_MENU_LINKS} "epartexe.exe libregman.so" "RegMan")
# ENDIF(WITH_REGMAN)

# This must always be last!
INCLUDE(CPack)
