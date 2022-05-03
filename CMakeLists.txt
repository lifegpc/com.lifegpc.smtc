cmake_minimum_required(VERSION 3.1)

project(smtc_binding VERSION 1.0.0)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include("VC-LTL helper for cmake")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_compile_definitions(_DISABLE_DEPRECATE_LTL_MESSAGE)

find_package(Qt5 COMPONENTS Core Quick Qml QuickControls2 REQUIRED)

set(ENABLE_ICONV OFF CACHE BOOL "Libiconv is not needed.")
add_subdirectory(utils)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/utils")

set(PLUGIN_SOURCES
src/DllMain.h
src/DllMain.cpp
src/SMTCBinding.h
src/SMTCBinding.cpp
src/SMTCSession.h
src/SMTCSession.cpp
src/SMTCSessionManager.h
src/SMTCSessionManager.cpp
src/SMTCSMServer.h
src/SMTCSMServer.cpp
src/SocketHelper.h
src/SocketHelper.cpp
)

add_library(implplugin SHARED "${PLUGIN_SOURCES}")

target_link_libraries(implplugin PRIVATE Qt5::Core Qt5::Quick Qt5::Qml Qt5::QuickControls2 utils ws2_32)

set(PLUGIN_INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/qml/impl" CACHE PATH "Path to install directory")

install(TARGETS implplugin DESTINATION ${PLUGIN_INSTALL_DIR})
